#pragma once
#include "IMethodRequest.h"
#include "IFuture.h"
#include <list>
#include "Queue.h"

template <class ClassPtr>
class MethodRequest : public IMethodRequest
{
private: 
	
public:
	MethodRequest(ClassPtr& Servant ) : m_jobCount(0),
						  m_taskStatus(STOPPED),
						  m_pFuture(0),
						  m_taskPriority(LOW_PRIORITY),
						  m_pServant(Servant)
	{};
	MethodRequest(IFuture *pFuture,ClassPtr& Servant )  : m_jobCount(0),
									  m_taskStatus(STOPPED),
									  m_pFuture(pFuture), //just copy the pointers
									  m_taskPriority(LOW_PRIORITY),
			  						  m_pServant(Servant)
	{
	};
	~MethodRequest(void)
	{
		clearJobs();
	};

	PriorityType_t getPriority()
	{
		return m_taskPriority;
	};

	void setPriority(PriorityType_t priority)
	{
		m_taskPriority = priority;
	};

	TaskStatus_t checkTaskStatus()
	{
		return m_taskStatus;
	}

	void updateTaskStatus(TaskStatus_t status)
	{
		m_taskStatus = status;
	};

	void notifyFuture()
	{
		//TODO: notify the future
	};

	/**
	* Checks the status of the subtasks without modifying the subtasks
	* @return number of subtasks remaining (not completed)
	*/
	int checkSubtaskStatus() const
	{
		return static_cast<int>(m_qJobs.size() - m_jobCount);	
	};

	void setJobParameters( IFunctor* pFunc )
	{
		m_qJobs.push(pFunc);
	}

	int doJob()
	{
		if(!m_qJobs.empty())
		{
			IFunctor* pFunct = m_qJobs.front();
			
			if( (pFunct != 0) && (pFunct->getState() == NOT_EXECUTED) )
			{
				m_qJobs.pop(); 
				//execute the method
				if ( pFunct->getThreadAddressing() == ATH_SCHEDULER )
				{
					(*pFunct)();
				}
				else
				{
					//set job to corresponding thread
				}

				//increment job counter
				++m_jobCount;
				//put the executed method at the end of the queue
				m_qJobs.push(pFunct);
			}
		}
		//if job count equals total number of jobs -> all jobs are done
		//otherwise return jobs left
		int retVal = static_cast<int>( m_qJobs.size() - m_jobCount );
		
		return retVal;
	};

	/**
	* Clears jobs from job queue. 
	*/
	void clearJobs()
	{
		while(!m_qJobs.empty())
		{
			IFunctor* pFunct = m_qJobs.front();
			m_qJobs.pop();
			delete pFunct;
			pFunct = 0;
		}
	};

	IFuture* getFuture()
	{
		return m_pFuture;
	};

	bool checkFinishedExecution()
	{
		bool retVal = true;
		
		list<IFunctor*>::iterator it = std::find_if(m_qJobs.getInnerContainer().begin(), m_qJobs.getInnerContainer().end(), SearchNotExecuted());

		if(it != m_qJobs.getInnerContainer().end())
		{
			//found job not executed
			retVal = false;
		}

		return retVal;
	};

private:
	typedef CQueue<IFunctor*, list<IFunctor*> > QChildJobs_t;

	TaskStatus_t m_taskStatus;		/**< overall task status: started, stopped, finished */
	IFuture* m_pFuture;				/**< pointer to future to be filled */
	PriorityType_t m_taskPriority;	/**< priority of this task */
	ClassPtr&				m_pServant;
	int						m_jobCount;				/**< counts jobs that are not done */
	QChildJobs_t			m_qJobs;/**< queue to hold child jobs */

	struct SearchNotExecuted
	{
		bool operator()(IFunctor* pFunctor)
		{
			bool bRetVal = false;
			if(pFunctor != NULL)
			{
				//search for jobs that are not executed
				bRetVal	= (pFunctor->getState() == NOT_EXECUTED) || (pFunctor->getState() == SCHEDULED);
			}
			return bRetVal;
		}
	};

};