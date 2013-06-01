#pragma once
#include <assert.h>

/*
	Policies for ParameterCopying
*/

class IFunctor;

struct FunctorParamNoCopy
{
	static void copyParams(IFunctor* pDstFunctor, const IFunctor* pSrcFunctor)
	{};

};

#if 0
template <unsigned int paramIdDst, unsigned int paramIdSrc>
struct FunctorParamCopyStrategy
{
	/**
	* Actual copy algorithm
	*/
	static void copyParams(IFunctor* pDstFunctor, const IFunctor* pSrcFunctor)
	{
		assert(pDstFunctor);
		assert(pSrcFunctor);

		if( (pDstFunctor != 0) && (pSrcFunctor != 0) )
		{
			//out functor must not be executed
			//purpose of the alg: copy first job return params to second job input params
			if(pDstFunctor->getState() == NOT_EXECUTED)
			{
				pDstFunctor->setParamValue( paramIdDst, pSrcFunctor->getParamPtr(paramIdSrc) );
			}
		}
	}
};  
#endif // 0
