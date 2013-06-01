#pragma once
#include "FutureBase.h"
#include <vector>
#include "Servant.h"

//type of pointer to member functions
typedef bool (Servant::*fdoTask1)(wchar_t* );
typedef bool (Servant::*fdoTask2)( std::string , std::vector< std::string >& );

/**
* Proxy Class. 
* It is used by the test application to insert jobs
*/
class Proxy
{
public:
	typedef std::vector<std::string > stringVector;
	typedef stringVector::iterator stringVectorIter;
	typedef stringVector::const_iterator stringVectorCIter;
	
	Proxy(void);
	~Proxy(void);

	/* wrappers for async task calls from Servant */
	FutureBase<bool>* doTask1Wrapper(wchar_t* data);
	FutureBase<bool>* doTask2Wrapper( const std::string& str, stringVector& data);
private:
	Servant m_Servant; /**< pointer to servant */
};
