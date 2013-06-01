#pragma once

#include <windows.h>
#include "IFuture.h"
#include <vector>

/**
* Base class for future objects
* Should be specialized together with the method request to suite the client's needs
*/
template < class ResultType >
class FutureBase : public IFuture
{
public:
	FutureBase() 
	{
		InitializeCriticalSection(&m_csLock);
	};
	virtual ~FutureBase() 
	{
		DeleteCriticalSection(&m_csLock);
	};

	/**
	* Returns a reference to the inner array of results.
	* The deallocation of complex result types is left for the application
	*/
	const std::vector<ResultType>& getResults() const
	{
		return m_vResults;
	};

	/**
	* Inserts subjob result in result array.
	* Correspondence: subjob 1 <-> result 1
	* @param result Result of the subjob
	*/
	void addResult(ResultType result)
	{
		EnterCriticalSection(&m_csLock);
		
		m_vResults.push_back(result);

		LeaveCriticalSection(&m_csLock);
	};

	/**
	* Clears the results for the 
	*/
	void clearResults()
	{
		EnterCriticalSection(&m_csLock);

		m_vResults.clear();

		LeaveCriticalSection(&m_csLock);
	};

	/**
	* Returns last subjob result
	* @param rt Result returned
	* @return true if there is at least a result, false otherwise
	*/
	bool getLastResult(ResultType& rt)
	{
		bool bResAvailable=false;

		EnterCriticalSection(&m_csLock);

		if ( !m_vResults.empty() )
		{
			rt = *(m_vResults.rbegin());
			bResAvailable = true;
		}

		LeaveCriticalSection(&m_csLock);

		return bResAvailable;
	}

private:
	std::vector<ResultType> m_vResults; /**< array of subjob results */
	CRITICAL_SECTION m_csLock; /**< lock to synchronize access to result array */
};
