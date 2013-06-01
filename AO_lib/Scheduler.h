#pragma once

#include "IFunctor.h"

class IMethodRequest;

/**
* Scheduler class.
* Schedules jobs in activation queue and supervizes their execution
*/
class Scheduler
{
private:

	/**
	* Default constructor
	*/
	Scheduler(void);

	/**
	* Default destructor
	*/
	~Scheduler(void);

	static Scheduler* m_pInstance;		/**< unique instance of this singleton */

	HANDLE m_hSchedulerThreadHandle;	/**< scheduler thread handle */
	HANDLE m_hFinishEvent;				/**< Event to notify the MethodRequest a parallel job has finished*/
	HANDLE m_hSchEvent;					/**< event handle on which the scheduler will wait */

	bool m_bAlive;						/**< guard in order to kill the thread */

public:

	/**
	* Method to get the singleton instance
	* @return singleton instance
	*/
	static Scheduler* getInstance();

	/**
	* Method to destroy the singleton instance
	*/
	static void destroyInstance();

	/**
	* Wrapper for the scheduler's thread
	* @param thParam Parameter passed to the thread
	*/
	static DWORD WINAPI schedulerThreadProcWrapper(LPVOID thParam);

	/**
	* Actual thread procedure of the scheduler
	*/
	void schedulerThreadProc();

	/**
	* Creates scheduler thread and starts it
	* @return true for success, false for error
	*/
	bool initializeScheduler();

	/**
	* Deallocates all resources used by the scheduler
	*/
	void uninitializeScheduler();

	/**
	* Inserts a new request in the queue. This is synchronous (should be fast). ActivationQueue is thread safe.
	* It inserts directly in the ActivationQueue
	* @param pRequest Request to be inserted
	*/
	void insertTask(IMethodRequest* pRequest) const;

	void activateScheduler(bool bActive=true);

private:
	/* wrapper method to avoid copy-paste */
	void doHighPrioTask();
};
