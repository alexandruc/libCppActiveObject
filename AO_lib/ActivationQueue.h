#pragma once
#include <windows.h>
#include <queue>
#include "IMethodRequest.h"

using namespace std;


/**
* Class ActivationQueue
* Holds 2 queue containers: 1 for high priority jobs, and 1 for low priority jobs
*/
class ActivationQueue
{
private:

	/**
	* Default constructor
	*/
	ActivationQueue(void);

	/**
	* Default destructor
	*/
	~ActivationQueue(void);

	static ActivationQueue* m_pInstance; /**< singleton instance */

public:

	/**
	* Get the singleton instance
	* @return the singleton instance
	*/
	static ActivationQueue* getInstance();

	/**
	* Destroys the singleton instance
	*/
	static void destroyInstance();

	/**
	* Adds a task to the queue
	* @param task Task to add into the queue
	*/
	void enqueue(IMethodRequest* task);

	/**
	* Pops one task from the queue [Thread Safe]
	* @param priority task priority, in order to know from which queue to pop
	* @return Task popped. ATTENTION: task must be deallocated after it has filled the Future object
	*/
	IMethodRequest* dequeue(PriorityType_t priority);

	/**
	* Fetches the task in front of the queue (without popping)
	* @param priority task priority, in order to know from which queue to pop
	* @return Task in front of the queue (this does not pop the task from the queue)
	*/
	IMethodRequest* tryDequeue(PriorityType_t priority);

	/**
	* Checks the low prio queue for tasks
	* @return true for queue not empty, false for queue empty
	*/
	bool checkForLowPrioTasks();

	/**
	* Checks the high prio queue for tasks
	* @return true for queue not empty, false otherwise
	*/
	bool checkForHighPrioTasks();

	/**
	* Clears the queues: iterates through elements of the queue and deletes the MethodsRequests
	* TODO: should invalidate related Future objects
	*/
	void clearQueues();

private:
	typedef queue<IMethodRequest*> PriorityQueue_t;
	PriorityQueue_t m_qLowPriorityQueue;	/**< low priority queue */
	PriorityQueue_t m_qHighPriorityQueue;	/**<  high priority queue */

	CRITICAL_SECTION m_csLock; /**< lock in order to synchronize access */
};
