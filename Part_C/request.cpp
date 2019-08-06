#include "request.h"
#include "utilites.h"
#include <sstream>
#include <iomanip>
#include <string>
#include <algorithm>


using namespace std;

#ifdef _DEBUG
#include <test_runner.h>

void BuildRequest_test()
{
	{
		auto request = BuildRequest("Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye");
		ASSERT(request->GetType() == InputBus);
	}
	{
		auto request = BuildRequest("Bus 256");
		ASSERT(request->GetType() == OutputBus);
	}
	{
		auto request = BuildRequest("Stop Tolstopaltsevo: 55.611087, 37.20829");
		ASSERT(request->GetType() == InputStop);
	}
}

#endif // _DEBUG



//===============================================
unique_ptr<Request> BuildRequest(string str)
{
	if (str.empty())
		return make_unique<Request>("");

	auto first_word = GetPrefix(str);

	auto it = find(str.begin(), str.end(), ':');
	bool is_output_type = (it == str.end()) ? true : false;

	if (first_word == "Stop")
	{
		if (is_output_type)
		{
			return make_unique<OutputStop_Request>("", str);
		}
		else
		{
			// Stop Biryulyovo Tovarnaya: 55.592028, 37.653656
			auto stop_name = GetPrefix(str, ':');
			return make_unique<InputStop_Request>(str, stop_name);
		}
	}
	else
	{
		if (is_output_type)
		{
			return make_unique<OutputBus_Request>("", string(str));
		}
		else
		{
			auto number = GetPrefix(str, ':');
			return make_unique<InputBus_Request>(str, number);
		}
	}
}
// InputBus_Request =============================
InputBus_Request::InputBus_Request(const string & str, const string & number)
	: bus_number(number), Request(str)
{
	type = InputBus;
	auto it = find(message.begin(), message.end(), '-');
	info.isCircle = (it != message.end());
	char delim = info.isCircle ? '-' : '>';
	info.bus_stops = SplitString(message, delim);

	for (auto &stop : info.bus_stops)
	{
		ClearPrefix(stop, ' ');
		ClearSufix(stop, ' ');
	}

	info.stops_on_route = info.isCircle ? (info.bus_stops.size() * 2) - 1 : info.bus_stops.size();
	info.uniq_stops = 0;

	for (auto it = info.bus_stops.begin(); it != info.bus_stops.end(); it++)
	{
		if (count(it, info.bus_stops.end(), *it) == 1)
			info.uniq_stops++;
	}
}
//-----------------------------------------------
void InputBus_Request::Process(BusManager & storage)
{
	/*
	Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
	Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
	*/

	for (const auto& stop : info.bus_stops)
	{
		storage.AddBusToStopList(stop, bus_number);
	}
	storage.AddBus(bus_number, info);
}
// InputStop_Request ============================
InputStop_Request::InputStop_Request(const string & str, const string & _stop_name) 
	: Request(str), stop_name(_stop_name)
{
	// Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam
	type = InputStop;
	auto vec = SplitString(message, ',');
	if (vec.size() < 2)
		throw invalid_argument("Request exception");

	info.position = Point(stod(vec[0].data()), stod(vec[1].data()));
	info.road_distances = GetStopesMap(vec);
}
//-----------------------------------------------
void InputStop_Request::Process(BusManager & storage)
{
	storage.AddStop(stop_name, info);
}
// OutputBus_Request ============================
OutputBus_Request::OutputBus_Request(const string & str, const string & number) 
	: bus_number(number), Request("")
{
	type = OutputBus;
}
//-----------------------------------------------
void OutputBus_Request::Process(BusManager & storage)
{
	stringstream stream;
	if (storage.IsHasBus(bus_number))
	{
		const auto &bus_info = storage.GetBus(bus_number);


		double dist_direct = storage.GetDirectDistance(bus_number);
		int dist_real = storage.GetRealRouthLength(bus_number);
		double curvature = dist_real / dist_direct;
		// Bus 750: 5 stops on route, 3 unique stops, 27600 route length, 1.318084 curvature
		// Bus 751 : not found
		if (precision > 0)
			stream << setprecision(precision);
		stream
			<< "Bus " << bus_number << ": "
			<< bus_info.stops_on_route << " stops on route, "
			<< bus_info.uniq_stops << " unique stops, "
			<< dist_real << " route length, "
			<< curvature << " curvature";
	}
	else
	{
		stream << "Bus " << bus_number << ": not found";
	}
	message = stream.str();
}
// OutputStop_Request ===========================
OutputStop_Request::OutputStop_Request(const string & str, const string & _stop_name) 
	: Request(""), stop_name(_stop_name)
{
	type = OutputStop;
}
//-----------------------------------------------
void OutputStop_Request::Process(BusManager & manager)
{
	//		Stop Prazhskaya : no buses
	//		Stop Biryulyovo Zapadnoye : buses 256 828
	stringstream output;
	output << "Stop " << stop_name << ": ";

	if (!manager.IsHasStop(stop_name))
	{
		output << "not found";
	}
	else
	{
		const auto &buses = manager.GetStop(stop_name).stop_buses;
		if (buses.empty())
		{
			output << "no buses";
		}
		else
		{
			output << "buses";
			for (const auto &bus : buses)
				output << ' ' << bus;
		}
	}
	message = output.str();
}
//-----------------------------------------------


