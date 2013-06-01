#pragma once
#include <vector>
#include <string>

/**
* Dummy servant class. Emulates a class that executes the actual backround operations
*/
class Servant
{
public:
	Servant(void);
	~Servant(void);

	/* a task with 1 parameter */
	bool doTask1(wchar_t* data);

	/* a task with 2 parameters */
	bool doTask2( std::string str, std::vector<std::string >& data);
};
