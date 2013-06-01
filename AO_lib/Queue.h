#pragma once
#include <queue>

/**
* Wrapper class for STL queue - exposes inner queue container for some additional operations
*/
template <class T1, class T2>
class CQueue : public std::queue<T1, T2>
{
public:
	CQueue(void){};
	virtual ~CQueue(void){};

	T2& getInnerContainer()
	{
		return this->c;
	};
};

