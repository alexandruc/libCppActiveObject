#include "Proxy.h"
#include "IMethodRequest.h"
#include "MethodRequest.h"
#include "Scheduler.h"
#include "FunctorParamCopyPolicy.h"


Proxy::Proxy(void) : m_Servant()
{
	Scheduler::getInstance()->initializeScheduler();
}

Proxy::~Proxy(void)
{
	Scheduler::getInstance()->uninitializeScheduler();
}

FutureBase<bool>* Proxy::doTask1Wrapper(wchar_t *data)
{
	//creating future
	typedef DefaultCallPolicy<bool, Servant, fdoTask1, true> callPolicy;

	FunctorBase1<callPolicy, wchar_t* >* doTask1Functor = new FunctorBase1<callPolicy, wchar_t* >( &m_Servant, &Servant::doTask1, data, NULL );
	IFuture& pFuture = doTask1Functor->getFuture();
	
	//attach future to the MR
	IMethodRequest* pTask = new MethodRequest<Servant>(&pFuture, m_Servant);

	//attach future to each subjob
	pTask->setJobParameters(doTask1Functor);

	//scheduler will take over from here
	Scheduler::getInstance()->insertTask(pTask);

	return static_cast<FutureBase<bool>*>(&pFuture);
}

FutureBase<bool>* Proxy::doTask2Wrapper( const std::string& str, stringVector& data)
{
	typedef DefaultCallPolicy<bool, Servant, fdoTask2, true> callPolicy1;
	//create future
	FutureBase<bool>* mrFuture = new FutureBase<bool>();
	
	//attach future to the MR
	IMethodRequest* pTask = new MethodRequest<Servant>(mrFuture, m_Servant);

	// build the child tasks for the MethodRequest -> split the data in 3 piece sub jobs
	for (stringVectorIter iter=data.begin(); iter!=data.end();)
	{
		// copy vector parts
		stringVectorIter iterEnd = data.end();
		int diff = std::distance<stringVectorIter>(iter,iterEnd);
		if ( diff > 3 )
		{
			iterEnd = iter + 3;
		}
		stringVector tempVector;
 		tempVector.insert( tempVector.begin(), iter, iterEnd );

		// creating child functions
		FunctorBase2<callPolicy1, std::string, stringVector >* pdoTask2Functor = new FunctorBase2<callPolicy1, std::string, stringVector >( &m_Servant, &Servant::doTask2, str, data);
		IFuture& pFuture = pdoTask2Functor->getFuture();
		pTask->setJobParameters(pdoTask2Functor);

		iter = iterEnd;
	}

	//scheduler will take over from here
	Scheduler::getInstance()->insertTask(pTask);

	return mrFuture;
}