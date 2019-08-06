#include "RequestManager.h"
#include "request.h"

using namespace std;

#ifdef _DEBUG
#include <test_runner.h>
#include <fstream>
const std::string test_folder = "tests//";
void Case1()
{
	ifstream in_file(test_folder + "case1_in.txt");
	ifstream expected(test_folder + "case1_out.txt");

	if (!in_file.is_open(), !expected.is_open())
		throw exception("case 1 failed! Can't open files.");

	BusManager bus_manager;
	stringstream output;
	RequestManager request_manager(in_file, output);
	request_manager.SetBusManager(&bus_manager);
	request_manager.SetPrecision(7);
	request_manager.ReadOrderedRequests();
	request_manager.PrintRecponces();

	string str_expected, buff;
	while (getline(expected, buff, '\n'))
	{
		str_expected += buff + "\n";
	}

	ASSERT_EQUAL(output.str(), str_expected);

}
#endif // _DEBUG


void RequestManager::ReadOrderedRequests()
{
	if (!bus_manager)
		throw runtime_error("bus_manager = nullptr");

	int count = 0;
	string buff;
	getline(input, buff, '\n');
	count = stoi(buff.c_str());
	unique_ptr<Request> request;
	for (int i = 0; i < count; i++) // читаем ввод
	{
		getline(input, buff, '\n');
		request = BuildRequest(buff);
		request->Process(*bus_manager);
	}

	getline(input, buff, '\n');
	count = stoi(buff.c_str());
	for (int i = 0; i < count; i++) // читаем вывод
	{
		getline(input, buff, '\n');
		request = BuildRequest(buff);
		if (precision > 0)
			request->SetPrecision(precision);
		request->Process(*bus_manager);
		if (request->GetType() > Request::Last_Input_Type)
			responces.push_back(request->GetMessage());
	}
}
//-----------------------------------------------
void RequestManager::PrintRecponces() const
{
	for (const auto &responce : responces)
	{
		output << responce << '\n';
	}
}