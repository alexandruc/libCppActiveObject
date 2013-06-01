#pragma once
#include <windows.h>
#include "ifuture.h"

typedef enum FunctorState_e
{
	NOT_EXECUTED,
	SCHEDULED,
	EXECUTED
} FunctorState_t;

/**
* Will be used in case of multiple background threads - not used right now
*/
enum eAddressedThread
{
	ATH_UNKNOWN=0,
	ATH_SCHEDULER
};

/**
* Base class for functors. It's purpose is to decouple the template mechanism from the ActivationQueue and the scheduler.
*/
class IFunctor
{
public:
	IFunctor(){};

	virtual ~IFunctor(){};

	/**
	* Operation to be implemented by base classes
	*/
	virtual void operator() () = 0;

	/**
	* Returns the state of the functor.
	* -EXECUTED -> inner object method was executed
	* -SCHEDULED -> functor is scheduled for execution
	* -NOT_EXECUTED -> inner method was not executed
	*/
	virtual FunctorState_t getState() const = 0;

	virtual void setState(FunctorState_t state) = 0;

	virtual int getParamNo() const =0;

	virtual bool functionSuccessful() const =0;

	virtual void copyParams(const IFunctor* functorSrc)=0;

	virtual IFuture& getFuture() const = 0;

	virtual eAddressedThread getThreadAddressing() const =0;

};