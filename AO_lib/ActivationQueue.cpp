#include "ActivationQueue.h"
#include "Scheduler.h"

ActivationQueue* ActivationQueue::m_pInstance = NULL;

ActivationQueue::ActivationQueue(void)
{
	InitializeCriticalSection( &m_csLock );
}

ActivationQueue::~ActivationQueue(void)
{
	DeleteCriticalSection( &m_csLock );
}

ActivationQueue* ActivationQueue::getInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new ActivationQueue();
	}
	return m_pInstance;
}

void ActivationQueue::destroyInstance()
{
	delete m_pInstance;
	m_pInstance = 0;
}

void ActivationQueue::enqueue(IMethodRequest *task)
{
	EnterCriticalSection( &m_csLock );

	if(task != NULL)
	{
		switch(task->getPriority())
		{
		case HIGH_PRIORITY:
			{
				m_qHighPriorityQueue.push(task);
			}
			break;
		case LOW_PRIORITY:
			{
				m_qLowPriorityQueue.push(task);
			}
			break;
		default:
			{
			}
			break;
		}
		Scheduler::getInstance()->activateScheduler();
	}

	LeaveCriticalSection( &m_csLock );
}

IMethodRequest* ActivationQueue::dequeue(PriorityType_t priority)
{
	EnterCriticalSection( &m_csLock );

	IMethodRequest* pTask = NULL;
	switch(priority)	
	{
	case LOW_PRIORITY:
		{
			pTask = m_qLowPriorityQueue.front();
			//pop should be done after all child tasks have finished
			m_qLowPriorityQueue.pop(); 
		}
		break;
	case HIGH_PRIORITY:
		{
			pTask = m_qHighPriorityQueue.front();
			m_qHighPriorityQueue.pop();
		}
		break;
	default:
		{}
		break;
	}
	if ( m_qLowPriorityQueue.size() + m_qHighPriorityQueue.size() == 0 )
	{
		Scheduler::getInstance()->activateScheduler(false);
	}

	LeaveCriticalSection( &m_csLock );

	return pTask;
}

IMethodRequest* ActivationQueue::tryDequeue(PriorityType_t priority)
{
	IMethodRequest* pTask = NULL;
	switch(priority)	
	{
	case LOW_PRIORITY:
		{
			if(!m_qLowPriorityQueue.empty())
			{
				pTask = m_qLowPriorityQueue.front();
			}
		}
		break;
	case HIGH_PRIORITY:
		{
			if(!m_qHighPriorityQueue.empty())
			{
				pTask = m_qHighPriorityQueue.front();
			}
		}
		break;
	default:
		{}
		break;
	}

	return pTask;
}

bool ActivationQueue::checkForHighPrioTasks()
{
	return !m_qHighPriorityQueue.empty();
}

bool ActivationQueue::checkForLowPrioTasks()
{
	return !m_qLowPriorityQueue.empty();
}

void ActivationQueue::clearQueues()
{
	IMethodRequest* pMR = NULL;
	//clear high prio queue
	while( !m_qHighPriorityQueue.empty() )
	{
		pMR = m_qHighPriorityQueue.front();
		m_qHighPriorityQueue.pop();
		delete pMR;
		pMR = NULL;
	}
	//clear low prio queue
	while( !m_qLowPriorityQueue.empty() )
	{
		pMR = m_qLowPriorityQueue.front();
		m_qLowPriorityQueue.pop();
		delete pMR;
		pMR = NULL;
	}
}