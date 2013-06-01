#include <windows.h>
#include "Proxy.h"
#include <vector>
#include <fstream>

#define USE_CHAR

using namespace std;

int main()
{
	Proxy *pProxy = new Proxy();
	IFuture* pdoTask1Future = 0;
	IFuture* pdoTask2Future = 0;
	Proxy::stringVector data;

	data.push_back("data1");
	data.push_back("data2");
	data.push_back("data3");
	data.push_back("data4");
	data.push_back("data5");
	data.push_back("data6");


	std::string str = "dummy";

	for(int i = 0; i< 10; ++i)
	{
		//1
		pdoTask1Future = pProxy->doTask1Wrapper(L"test data 1");
		pdoTask1Future = pProxy->doTask1Wrapper(L"test data 2");

		pdoTask2Future = pProxy->doTask2Wrapper(str, data);

		//2
		pdoTask1Future = pProxy->doTask1Wrapper(L"test data 3");

		pdoTask2Future = pProxy->doTask2Wrapper(str, data);

		//3
		pdoTask1Future = pProxy->doTask1Wrapper(L"test data 4");

		pdoTask2Future = pProxy->doTask2Wrapper(str, data);

	}

	getchar();

	delete pProxy;
	pProxy = 0;

	return 0;
}