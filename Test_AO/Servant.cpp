#include <Windows.h>
#include "Servant.h"
#include <stdio.h>


Servant::Servant(void)
{
}

Servant::~Servant(void)
{
}

bool Servant::doTask1(wchar_t *data)
{
	printf("Servant::doTask1: START -> starting job : data: %ls \n", data);
	//simulate some processing
	Sleep(2000);
	printf("Servant::doTask1: END \n");
	return true;
}

bool Servant::doTask2( std::string str, std::vector<std::string >& data)
{
	printf("Servant::doTask2: START -> starting job \n");
	std::vector<std::string>::iterator it = data.begin();
	std::vector<std::string>::iterator endit = data.end();
	while(it != endit)
	{
		printf("Servant::doTask2: data: %s \n", it->c_str());
		it++;
	}
	
	//simulate some processing
	Sleep(2);
	printf("Servant::doTask2: END \n");
	return true;
}