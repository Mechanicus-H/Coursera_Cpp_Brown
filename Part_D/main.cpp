// FinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "BusManager.h"
#include "RequestManager.h"
#include <iostream>

using namespace std;
//===============================================
#ifdef _DEBUG
#include <test_runner.h>
#include "Point.h"
#include "request.h"
#include "utilites.h"

#include "json.h"
#include <fstream>

void Play_with_Json()
{
	ifstream in("tests//case1_in.txt");
	BusManager bus_manager;
	RequestManager request_manager(in, cout);
	request_manager.SetBusManager(&bus_manager);
	request_manager.ReadJsonRequests();
	request_manager.ProcessRequests();
}


void RunAllTests()
{
	TestRunner tr;
	RUN_TEST(tr, Point_Test);
	RUN_TEST(tr, SpitString_test);
	RUN_TEST(tr, GetPrefix_test);
	RUN_TEST(tr, GetStopesMap_test);

	RUN_TEST(tr, BuildRequest_test);
	RUN_TEST(tr, ClearPrefixSuffix_test);
	RUN_TEST(tr, Case1);
	RUN_TEST(tr, Play_with_Json);
}
#endif // _DEBUG

int main()
{
#ifdef _DEBUG
	RunAllTests();
#endif // _DEBUG

}