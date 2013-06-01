#include "Scheduler.h"
#include "ActivationQueue.h"

#define DEFAULT_SCHEDULER_TIMEOUT 500 //wait 500ms before checking for a new task
#define DEFAULT_WAIT_THREAD_SHUTDOWN 2000 //time to wait for thread shutdown

Scheduler* Scheduler::m_pInstance = NULL;

Scheduler::Scheduler(void) : m_hSchedulerThreadHandle(0),
							m_hSchEvent(0),
							m_bAlive(true)
{
}

Scheduler::~Scheduler(void)
{
}

Scheduler* Scheduler::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new Scheduler();
	}
	return m_pInstance;
}

void Scheduler::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
}

bool Scheduler::initializeScheduler()
{
	bool bRetVal = true;

	m_hSchEvent = CreateEvent(NULL, FALSE /* auto reset */, FALSE /* non-signaled first */, L"SchedulerEvent");
	if(m_hSchEvent == NULL)
	{
		printf("FAILED to create scheduler event \n");
		bRetVal = false;
	}

	if(bRetVal)
	{
		DWORD dwThreadId = 0;
		m_hSchedulerThreadHandle = CreateThread(NULL, NULL, schedulerThreadProcWrapper, this, NULL, &dwThreadId);
		if(m_hSchedulerThreadHandle == NULL)
		{
			printf("FAILED to create scheduler thread \n");
			bRetVal = false;
			m_bAlive = false;
		}
		else
		{
			m_bAlive = true;
		}
	}

	return bRetVal;
}

void Scheduler::uninitializeScheduler()
{
	m_bAlive = false;
	SetEvent(m_hSchEvent);
	WaitForSingleObject(m_hSchedulerThreadHandle, DEFAULT_WAIT_THREAD_SHUTDOWN);
	CloseHandle(m_hSchedulerThreadHandle);
}

DWORD Scheduler::schedulerThreadProcWrapper(LPVOID thParam)
{
	if(thParam)
	{
		Scheduler* pThis = reinterpret_cast<Scheduler*>(thParam);
		pThis->schedulerThreadProc();
	}
	return 0;
}

void Scheduler::activateScheduler(bool bActive/*=true*/)
{
	if ( bActive )
	{
		::SetEvent(m_hSchEvent);
	}
	else
	{
		::ResetEvent(m_hSchEvent);
	}
}


void Scheduler::schedulerThreadProc()
{
	DWORD dwWaitVal = 0;
	ActivationQueue* pAQueue = ActivationQueue::getInstance();

	while(true)
	{
		dwWaitVal = WaitForSingleObject(m_hSchEvent, DEFAULT_SCHEDULER_TIMEOUT);
		if(!m_bAlive)
		{
			//gracefully kill the thread
			break;
		}

		switch(dwWaitVal)
		{
		case WAIT_TIMEOUT:
			printf("-->Scheduler::timeout!<--- \n");
		case WAIT_OBJECT_0:
			{
				//check if there are jobs that can be run
				if( pAQueue->checkForHighPrioTasks() )
				{
					doHighPrioTask();
				}
				else
				{
					if( pAQueue->checkForLowPrioTasks() )
					{
						IMethodRequest* pMR = pAQueue->tryDequeue(LOW_PRIORITY);
						while( pMR->doJob() != 0 )
						{
							//while executing low priority sub tasks, check for high priority jobs
							if( pAQueue->checkForHighPrioTasks() )
							{
								doHighPrioTask();
							}
						}
						if(pMR->checkFinishedExecution())
						{
							pMR->notifyFuture(); //job done -> write results to Future
							//remove MR from queue -> same object stored in pMR
							pAQueue->dequeue(LOW_PRIORITY);
							//delete MR;
							delete pMR;
							pMR = 0;
						}
					}
				}
			}
			break;
		default:
			{}
			break;

		}
	}
}

void Scheduler::insertTask(IMethodRequest *pRequest) const
{
	ActivationQueue::getInstance()->enqueue(pRequest);
}

void Scheduler::doHighPrioTask()
{
	ActivationQueue* pAQueue = ActivationQueue::getInstance();
	IMethodRequest* pMR = pAQueue->tryDequeue(HIGH_PRIORITY);
	while( pMR->doJob() != 0)
	{
	}
	//if all jobs were executed (all subjobs finished)
	if(pMR->checkFinishedExecution())
	{
		pMR->notifyFuture(); //job done -> write results to Future
		//dequeue and dispose of MR object
		pAQueue->dequeue(HIGH_PRIORITY);
		delete pMR;
		pMR = 0;
	}
}