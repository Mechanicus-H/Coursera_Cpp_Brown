#include "request.h"
#include "utilites.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <unordered_map>


using namespace std;
using namespace Json;
#ifdef _DEBUG
#include <test_runner.h>

void BuildRequest_test()
{
	
}

#endif // _DEBUG
//-----------------------------------------------
template<class T>
size_t CalcUniq(const vector<T> &vec)
{
	size_t result = 0;
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		auto c = count(it, vec.end(), *it);
		if (c == 1)
			result++;
	}
	return result;
}
//-----------------------------------------------
unordered_map<string, int> GetDistanceMap(const map<string, Node> node_map)
{
	unordered_map<string, int> result;
	for (auto &[name, dist] : node_map)
		result[name] = static_cast<int>(dist.AsDouble());
	return result;
}


//-----------------------------------------------
unique_ptr<Request> BuildRequest(const map<string, Node> &params)
{
	const string & sType = params.at("type").AsString();
	bool isInput = (params.count("id") == 0) ? true : false;
	if (sType == "Stop")
	{
		if (isInput)
			return make_unique<InputStop_Request>(params);
		else
			return make_unique<OutputStop_Request>(params);
	}
	else if (sType == "Bus")
	{
		if (isInput)
			return make_unique<InputBus_Request>(params);
		else
			return make_unique<OutputBus_Request>(params);
	}
	throw invalid_argument("BuildRequest fail!");
}
// Request ======================================
Request::Request(std::map<std::string, Json::Node> params_map)
{
	name = params_map.at("name").AsString();
	if (params_map.count("id") > 0)
		id = static_cast<int>(params_map.at("id").AsDouble());
	else
		id = 0;
}
// InputBus =====================================
InputBus_Request::InputBus_Request(const std::map<std::string, Json::Node>& params_map)
	: Request(params_map)
{
	info.isCircle = params_map.at("is_roundtrip").AsBool();
	for (auto &stop : params_map.at("stops").AsArray())
		info.bus_stops.push_back(stop.AsString());
	info.stops_on_route = (info.isCircle) ? (info.bus_stops.size() * 2) - 1 : info.bus_stops.size();
	info.uniq_stops = CalcUniq(info.bus_stops);
}
//-----------------------------------------------
void InputBus_Request::Process(BusManager & storage)
{
	storage.AddBus(name, info);
}
// InputStop ====================================
InputStop_Request::InputStop_Request(const std::map<std::string, Json::Node>& params_map)
	: Request(params_map)
{
	info.position = Point(
		params_map.at("latitude").AsDouble(),
		params_map.at("longitude").AsDouble()
	);
	if (params_map.count("road_distances") > 0)
		info.road_distances = GetDistanceMap(params_map.at("road_distances").AsMap());
}
//-----------------------------------------------
void InputStop_Request::Process(BusManager & storage)
{
	storage.AddStop(name, info);
}
// OutputBus ====================================
OutputBus_Request::OutputBus_Request(const std::map<std::string, Json::Node> params_map)
	: Request(params_map)
{}
//-----------------------------------------------
void OutputBus_Request::Process(BusManager & storage)
{
	responce_map["request_id"] = Node(double(id));
	if (storage.IsHasBus(name))
	{
		const auto &info = storage.GetBus(name);
		auto route_length = storage.GetRealRouthLength(name);
		auto direct_distance = storage.GetDirectDistance(name);

		responce_map["route_length"] = Node(double(route_length));
		responce_map["stop_count"] = Node(double(info.stops_on_route));
		responce_map["unique_stop_count"] = Node(double(info.uniq_stops));
		responce_map["curvature"] = route_length / direct_distance;
	}
	else
		responce_map["error_message"] = Node("not found");
}
//-----------------------------------------------

// OutputStop ===================================
OutputStop_Request::OutputStop_Request(const std::map<string, Json::Node> params)
	: Request(params)
{}
//-----------------------------------------------
void OutputStop_Request::Process(BusManager & manager)
{
	responce_map["request_id"] = Node(double(id));
	if (manager.IsHasStop(name))
	{
		const auto &info = manager.GetStop(name);
		vector<Node> buses;
		for (const auto &bus : info.stop_buses)
			buses.push_back(Node(bus));
		responce_map["buses"] = move(buses);
	}
	else
		responce_map["error_message"] = Node("not found");
}
//-----------------------------------------------