#pragma once
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include "Point.h"

struct BusInfo
{
	bool isCircle;
	std::vector<std::string> bus_stops;
	size_t stops_on_route;
	size_t uniq_stops;
};
//-----------------------------------------------
struct StopInfo
{
	Point position;
	std::set<std::string> stop_buses;
	std::unordered_map<std::string, int> road_distances;
	void Merge(const StopInfo &other)
	{
		if (position.IsNull()) position = other.position;
		stop_buses.insert(other.stop_buses.begin(), other.stop_buses.end());
		road_distances.insert(other.road_distances.begin(), other.road_distances.end());
	}
};
//-----------------------------------------------
class Request;

class BusManager
{
private:
	std::unordered_map<std::string, BusInfo> buses;
	std::unordered_map<std::string, StopInfo> bus_stops;

	BusManager(const BusManager&) = delete;
	BusManager(BusManager&&) = delete;

public:
	BusManager() {}
	const auto& GetBuses() const { return buses; }
	const auto& GetBusStops() const { return bus_stops; }

	const auto& GetBus(const std::string &bus_name) const { return buses.at(bus_name); }
	const auto& GetStop(const std::string &stop_name) const { return bus_stops.at(stop_name); }

	void AddBusToStopList(const std::string &stop_name, const std::string &bus_name); // добавить в транспорт в список транспорта остановки
	void AddCoordinates(const std::string &stop_name, Point coord); // ƒобавить координаты к инфо остановки
	void AddStopToBusRoute(const std::string &bus_name, const std::string &stop_name); // добавить остановку в маршрут транспорта

	void AddBus(const std::string &bus_name, const BusInfo &info);
	void AddStop(const std::string &stop_name, const StopInfo &info);
	void AddDistance(const std::string& stop_from, const std::string &stop_to, int dist);

	bool IsHasBus(const std::string &bus_number) const;
	bool IsHasStop(const std::string &stop_name) const;

	int GetRealDistance(const std::string &from, const std::string &to) const;

	double GetDirectDistance(const std::string &bus_number) const;
	int GetRealRouthLength(const std::string &bus_number) const;
};