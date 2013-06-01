#pragma once
#include "IFunctor.h"
#include "FunctorBase.h"
#include "IFuture.h"
#include <queue>

using namespace std;

typedef enum PriorityType_e
{
	LOW_PRIORITY,
	HIGH_PRIORITY
} PriorityType_t;

typedef enum TaskStatus_e
{
	STARTED,
	STOPPED,
	FINISHED
} TaskStatus_t;

/**
* Contract for implementing IMethodRequests
* Restrictions:
* - one method request should be owned by only one thread
* - this class is not synchronized
*/

class IMethodRequest
{
public:
	IMethodRequest()
	{};
	virtual ~IMethodRequest() 
	{};

	/**
	* Returns this task's priority. To be implemented by concrete class.
	* @return task priority
	*/
	virtual PriorityType_t getPriority() = 0;

	/**
	* Checks current task status (whole task). To be implemented by concrete class.
	* @return Task status
	*/
	virtual TaskStatus_t checkTaskStatus() = 0;

	/**
	* Updates current task status (whole task). To be implemented by concrete class.
	* @param status new status
	*/
	virtual void updateTaskStatus(TaskStatus_t status) = 0;

	/**
	* Checks the status of the subtasks without modifying the subtasks
	* @return number of subtasks remaining (not completed)
	*/
	virtual int checkSubtaskStatus() const  =0;

	/**
	* Each call to these functions adds a new child job to the method request
	*/
	virtual void setJobParameters( IFunctor* pFunc )=0;

	/**
	* Performs one subtask, and puts it at the end of the queue
	* @return number of subtasks remaining (not completed)
	*/
	virtual int doJob()=0;

	/**
	* Notifies Future object. Should also insert job result in future object. 
	* To be implemented by concrete method request.
	*/
	virtual void notifyFuture() = 0;

	/**
	* Clears jobs from job queue. 
	*/
	virtual void clearJobs() = 0;

	/**
	* Tries to clean up resources for the current object. 
	* Purpose: avoid deleting MR before all subjobs are finished
	* @return true if all subjobs are executed and could be cleared, false otherwise
	*/
	virtual bool checkFinishedExecution() = 0;

protected:													
};