#pragma once
#include <windows.h>
#include <assert.h>
#include "IFunctor.h"
#include "FutureBase.h"

template<class T, class K> class FunctorBase;


template <typename FunRetType, class ClassPtr, typename FunPtr, FunRetType SuccessValue, typename eAddressedThread threadId=ATH_SCHEDULER>
struct DefaultCallPolicy
{
	typedef ClassPtr _callpol_CP;
	typedef FunPtr _callpol_FPtr;
	typedef FunRetType _callpol_FRT;

	bool evalResult(const IFunctor& fb) const
	{
		bool bSuccess=false;
		FunRetType rt=SuccessValue;
		if ( static_cast<FutureBase<FunRetType>& >(fb.getFuture()).getLastResult(rt) && rt == SuccessValue )
		{
			bSuccess = true;
		}

		return bSuccess;
	}

	eAddressedThread getThreadId() const {return threadId;}
};  


/**
* Base class for functors. Defines a functor with 1 method that has 0 parameters and returns FunRetType (must be != void)
* This class encapsulates the action that has to be executed
*/
template <class callPolicy, class CopyParameterPolicy=FunctorParamNoCopy>
class FunctorBase : public IFunctor
{
protected:
	callPolicy m_CallPolicy;

protected:
	/**
	* Default constructor
	*/
	FunctorBase(void)
	{};

public:
	typedef typename callPolicy::_callpol_CP	_CLSPTR;
	typedef typename callPolicy::_callpol_FPtr	_CLSFOOPTR;
	typedef typename callPolicy::_callpol_FRT	_FUNRETTYPE;


	/**
	* Constructor with parameters
	* @param pClass pointer to the class whose method is to be called
	* @param pf pointer to the member function of the class specified by pClass
	* @param pDependantJob pointer to the action that this class depends on (link will be 1 to 1)
	*/
	FunctorBase(_CLSPTR* pClass, _CLSFOOPTR pf) 
				:	
				m_FunctorState(NOT_EXECUTED),
				m_pf(pf),
				m_pClass(pClass),
				m_Future()
				
	{
	};

	virtual ~FunctorBase(void)
	{};

	virtual void operator()()
	{
		// this call should be synchronous
		//m_retVal = m_pf();
		//subclasses should override this method with the call using the appropriate parameters
		m_FunctorState = EXECUTED;
	};

	IFuture& getFuture() const
	{
		return const_cast<FutureBase<_FUNRETTYPE>&> (m_Future);
	}

	/**
	* Returns the state of the functor
	* @return state of the functor: EXECUTED/NOT_EXECUTED/SCHEDULED
	*/
	FunctorState_t getState() const
	{
		return m_FunctorState;
	};

	/**
	* Updates the current state of the functor
	* @param state New functor state
	*/
	void setState(FunctorState_t state)
	{
		m_FunctorState = state;
	};

	/**
	* Returns the return value of the method that was executed. Should be called only when state is EXECUTED.
	*/
	_FUNRETTYPE getMethodReturnValue() const
	{
		return m_retVal;
	}

	/**
	* Returns number of parameters
	*/
	virtual int getParamNo() const 
	{
		return 0;
	}

	virtual bool functionSuccessful() const
	{
		return m_CallPolicy.evalResult(*this);
	}

	virtual eAddressedThread getThreadAddressing() const
	{
		return m_CallPolicy.getThreadId();
	}

	void copyParams(const IFunctor* functorSrc)
	{
		CopyParameterPolicy::copyParams(this, functorSrc);
	}

protected:
	_CLSPTR*				m_pClass;				/**< pointer to the class which contains the method to be called */
	_CLSFOOPTR				m_pf;					/**< pointer to class method that this object will execute */
	_FUNRETTYPE				m_retVal;				/**< result that the action will return (will be filled in the future) */
	FunctorState_t			m_FunctorState;			/**< State of the action: executed or not executed */ 
	FutureBase<_FUNRETTYPE> m_Future;
};


/**
* Defines a functor with 1 method that has 1 parameter and returns FunRetType (must be != void)
*/
template <class callPolicy, class T1, class CopyParameterPolicy=FunctorParamNoCopy>
class FunctorBase1 : public FunctorBase<callPolicy, CopyParameterPolicy>
{
protected:
	FunctorBase1(void) : m_pFuture(0)
	{};	

public:

	/**
	* Constructor with parameters
	* @param pClass pointer to the class whose method is to be called
	* @param pf pointer to the member function of the class specified by pClass
	* @param param1 method parameter - used when calling pClass->pf(param1)
	* @param pDependantJob pointer to the action that this class depends on (link will be 1 to 1 and the strategy will be handled in copy parameter strategy)
	*/
	FunctorBase1( _CLSPTR* pClass, _CLSFOOPTR pf, const T1& param1, IFunctor* pDependantJob ) 
		:	
		FunctorBase(pClass, pf),
		m_param1(param1)
	{
	};

	virtual ~FunctorBase1(void)
	{};

	/**
	* Overloaded operator ()
	* Calls method from object passed in the constructor. Returns void, fills future object
	*/
	virtual void operator()()
	{
		// this call should be synchronous
		m_retVal = (m_pClass->*m_pf)(m_param1);
		m_Future.addResult(m_retVal);
		m_FunctorState = EXECUTED;
	};

	/**
	* Returns the number of parameters for this functor
	* @return number of parameters
	*/
	virtual int getParamNo() const 
	{
		return 1;
	}

private:
	T1 m_param1; /**< member to store the method parameter deallocation handled by the default constructor, on class destroy*/
};



/**
* Defines a functor with 1 method that has 2 parameters and returns _FUNRETTYPE (must be != void)
*/
template <class callPolicy, class T1, class T2, class CopyParameterPolicy=FunctorParamNoCopy>
class FunctorBase2 : public FunctorBase<callPolicy, CopyParameterPolicy>
{
protected:
	FunctorBase2(void) : m_pFuture(0)
	{};

public:

	/**
	* Constructor with parameters
	* @param pClass pointer to the class whose method is to be called
	* @param pf pointer to the member function of the class specified by pClass
	* @param param1 method parameter - used when calling pClass->pf(param1, param2)
	* @param param2 method parameter - used when calling pClass->pf(param1, param2)
	* @param pDependantJob pointer to the action that this class depends on (link will be 1 to 1)
	*/
	FunctorBase2(_CLSPTR* pClass, _CLSFOOPTR pf, T1 param1, T2 param2) 
		: 
		FunctorBase( pClass, pf),
		m_param1(param1),
		m_param2(param2)
	{
	};

	virtual ~FunctorBase2(void)
	{};

	/**
	* Overloaded operator ()
	* Calls method from object passed in the constructor. Returns void, fills future object
	*/
	virtual void operator()()
	{
		// this call should be synchronous
		m_retVal = (m_pClass->*m_pf)(m_param1, m_param2);
		m_Future.addResult(m_retVal);
		m_FunctorState = EXECUTED;
	};

	/**
	* Returns the number of parameters for this functor
	* @return number of parameters
	*/
	virtual int getParamNo() const 
	{
		return 2;
	}

private:
	T1 m_param1; /**< copy of first parameter of the method */
	T2 m_param2; /**< copy of second parameter of the method */
};