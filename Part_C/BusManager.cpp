#include "BusManager.h"

using namespace std;

//-----------------------------------------------
void BusManager::AddBusToStopList(const string & stop_name, const string & bus_name)
{
	bus_stops[stop_name].stop_buses.insert(bus_name);
}
//-----------------------------------------------
void BusManager::AddStopToBusRoute(const string & bus_name, const string & stop_name)
{
	buses[bus_name].bus_stops.push_back(stop_name);
}
//-----------------------------------------------
void BusManager::AddBus(const string & bus_name, const BusInfo &info)
{
	buses[bus_name] = info;
}
//-----------------------------------------------
void BusManager::AddStop(const string & stop_name, const StopInfo &info)
{
	bus_stops[stop_name].Merge(info);
}
//-----------------------------------------------
void BusManager::AddDistance(const string & stop_from, const string & stop_to, int dist)
{
	bus_stops[stop_from].road_distances[stop_to] = dist;
}
//-----------------------------------------------
void BusManager::AddCoordinates(const string & stop_name, Point coord)
{
	bus_stops[stop_name].position = coord;
}
//-----------------------------------------------
bool BusManager::IsHasBus(const string & bus_number) const
{
	return (buses.count(bus_number) > 0);
}
//-----------------------------------------------
bool BusManager::IsHasStop(const string & stop_name) const
{
	return (bus_stops.count(stop_name) > 0);
}
//-----------------------------------------------
int BusManager::GetRealDistance(const string & from_stop, const string & to_stop) const // Реальная дистанция между двумя остановками, если нет то прямая дистанция
{
	if (IsHasStop(from_stop) && IsHasStop(to_stop))
	{
		const auto &dist_map_from = bus_stops.at(from_stop).road_distances;
		const auto &dist_map_to = bus_stops.at(to_stop).road_distances;
		int dist = 0;
		if (dist_map_from.count(to_stop) > 0)
			dist = dist_map_from.at(to_stop);
		if (dist == 0 && dist_map_to.count(from_stop) > 0)
			dist = dist_map_to.at(from_stop);
		if (dist > 0)
			return dist;
	}

	throw runtime_error("GetRealDistance error");
}
//-----------------------------------------------
double BusManager::GetDirectDistance(const string & bus_number) const
{
	double distance = 0.0;
	if (IsHasBus(bus_number))
	{
		const auto &bus_info = GetBus(bus_number);

		Point prev_point;
		double mult = bus_info.isCircle ? 2.0 : 1.0;
		for (size_t i = 0; i < bus_info.bus_stops.size(); i++)
		{
			if (i == 0)
			{
				prev_point = GetStop(bus_info.bus_stops.at(i)).position;
				continue;
			}

			const Point &curr_point = GetStop(bus_info.bus_stops.at(i)).position;
			distance += Point::GetDistance(prev_point, curr_point) * mult;
			prev_point = curr_point;
		}
	}
	return distance;
}
//-----------------------------------------------
int BusManager::GetRealRouthLength(const string & bus_number) const
{
	int distance = 0;
	if (IsHasBus(bus_number))
	{

		const auto &bus_info = GetBus(bus_number);

		string prev_stop;
		for (size_t i = 0; i < bus_info.bus_stops.size(); i++)
		{
			if (i == 0)
			{
				prev_stop = bus_info.bus_stops.at(i);
				continue;
			}

			const string &curr_stop = bus_info.bus_stops.at(i);

			distance += GetRealDistance(prev_stop, curr_stop);
			if (bus_info.isCircle)
				distance += GetRealDistance(curr_stop, prev_stop);
			prev_stop = curr_stop;
		}
	}

	return distance;
}
//-----------------------------------------------