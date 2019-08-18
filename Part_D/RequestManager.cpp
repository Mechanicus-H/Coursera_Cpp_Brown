#include "RequestManager.h"

using namespace std;
using namespace Json;

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

	

}
#endif // _DEBUG

//====================================================
void RequestManager::ReadJsonRequests()
{
	document = Load(input);
}
//-----------------------------------------------
void RequestManager::ProcessRequests()
{
	vector<Node> responces;
	const auto &root_map = document.GetRoot().AsMap();
	if (root_map.count("base_requests") == 0 ||
		root_map.count("stat_requests") == 0)
		throw runtime_error("Can't read Json");
	for (const auto & node : root_map.at("base_requests").AsArray())
	{
		auto request = BuildRequest(node.AsMap());
		request->Process(*bus_manager);
	}
	for (const auto &node : root_map.at("stat_requests").AsArray())
	{
		auto request = BuildRequest(node.AsMap());
		request->Process(*bus_manager);
		responces.push_back(request->GetResponce());
	}
	responce_doc.SetRoot(Node(responces));
}
//-----------------------------------------------

//-----------------------------------------------
void RequestManager::PrintRecponces() const
{
	
}