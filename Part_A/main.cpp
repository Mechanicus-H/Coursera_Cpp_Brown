#include <iostream>
#include <unordered_map>
#include <string_view>
#include <memory>
#include <list>
#include <vector>
#include <utility>
#include <math.h>
#include <iomanip>
#include <functional>
#include <memory>
#include <fstream>
#include <algorithm>

#ifdef _DEBUG
#include <test_runner.h>
#include <profile.h>

const std::string test_folder = "tests//";

#endif // _DEBUG


using namespace std;
// Utilites =====================================
vector<string> SplitString(const string &source, char delim = ' ')
{
	vector<string> result;
	if (source.empty())
		return result;
	size_t index = 0;
	do
	{
		auto pos = source.find(delim, index);
		if (pos - index > 0)
			result.push_back(source.substr(index, pos - index));
		index = pos + 1;

	} while (index != 0);

	if (result.back().empty())
		result.pop_back();

	return result;
}
//-----------------------------------------------
string GetPrefix(string &source, char delim = ' ')
{
	auto index = source.find(delim, 0);
	if (index < source.size())
	{
		auto result = source.substr(0, index);
		source.erase(0, index + 1);
		return result;
	}
	return "";
}
//-----------------------------------------------
void ClearSufix(string &str, char ch)
{
	if (str.empty())
		return;
	while (str.back() == ch)
		str.pop_back();
}
//-----------------------------------------------
void ClearPrefix(string &str, char ch)
{
	if (str.empty())
		return;
	auto it = find_if(str.begin(), str.end(), [=](char c) {return c != ch; });
	if (it != str.end())
		str.erase(str.begin(), it);

}
// Utilites tests ===============================
#ifdef _DEBUG
void SpitString_test()
{
	{
		string str("first second third");
		vector<string> expected = { "first", "second", "third" };
		ASSERT(expected == SplitString(str));
		str = "first second third   ";
		ASSERT(expected == SplitString(str));
		str = "   first   second  third   ";
		ASSERT(expected == SplitString(str));
	}
	{
		string str("first-second-third");
		vector<string> expected = { "first", "second", "third" };
		ASSERT(expected == SplitString(str, '-'));
		str = "first-second-third----";
		ASSERT(expected == SplitString(str, '-'));
		str = "----first---second-third----";
		ASSERT(expected == SplitString(str, '-'));
	}
}
//-----------------------------------------------
void GetPrefix_test()
{
	{
		string test_str("test string");
		auto result = GetPrefix(test_str);
		ASSERT_EQUAL(result, "test");
		ASSERT_EQUAL(test_str, "string");

		test_str = " test string";
		result = GetPrefix(test_str);
		ASSERT_EQUAL(result, "");
		ASSERT_EQUAL(test_str, "test string");

		test_str = "test_string";
		result = GetPrefix(test_str);
		ASSERT_EQUAL(result, "");
		ASSERT_EQUAL(test_str, "test_string");
	}
	
}
//-----------------------------------------------
void ClearPrefixSuffix_test()
{
	string str = "  test string  ";
	ClearPrefix(str, ' ');
	ASSERT_EQUAL(str, "test string  ");
	ClearSufix(str, ' ');
	ASSERT_EQUAL(str, "test string");
}
#endif // _DEBUG
//===============================================
// Point ========================================
constexpr const double PI = 3.1415926535;
constexpr const double EARTH_RAD = 6371000.0;

class Point
{
private:
	double
		lat, // широта в радинах
		lon; // долгота в радианах

	static pair<double, double> RadiansFromAngles(double lat, double lon);
public:
	Point() {}
	Point(double _lat, double _lon);
	Point(const Point& other) = default;
	
	const Point& operator = (const Point &p);
	
	pair<double, double> GetCoord() const; // {lat, lon}
	static double GetDistance(const Point& l, const Point& r);
	bool operator == (const Point& p2) const;
	friend ostream& operator << (ostream& out, const Point& p);
};
//-----------------------------------------------
pair<double, double> Point::RadiansFromAngles(double lat, double lon)
{
	return make_pair(
		(lat * PI) / 180,
		(lon * PI) / 180
	);
}
//-----------------------------------------------
Point::Point(double _lat, double _lon)
{
	auto [lat_rad, lon_rad] = RadiansFromAngles(_lat, _lon);
	lat = lat_rad;
	lon = lon_rad;
}
//-----------------------------------------------
const Point & Point::operator=(const Point & p)
{
	lat = p.lat;
	lon = p.lon;
	return *this;
}
//-----------------------------------------------
pair<double, double> Point::GetCoord() const
{
	return make_pair(lat, lon);
}
//-----------------------------------------------
double Point::GetDistance(const Point& l, const Point& r) // Дистанция в метрах
{
	return
		acos(
			sin(l.lat) * sin(r.lat) +
			cos(l.lat) * cos(r.lat) *
			cos(abs(l.lon - r.lon))
		) * EARTH_RAD;
}
//-----------------------------------------------
bool Point::operator==(const Point& p2) const
{
	return lat == p2.lat && lon == p2.lon;
}
//-----------------------------------------------
ostream& operator<<(ostream& out, const Point& p)
{
	out << "Point(" << p.lat << ", " << p.lon << ")";
	return out;
}
// Point Tests and Profile ======================
#ifdef _DEBUG
void Point_Test()
{
	{
		Point
			p1(15.14, 13.22),
			p2(15.14, 13.22);
		ASSERT_EQUAL(p1, p2);
	}
	{
		Point p1(68.28, 100.22), p2(40.71, -74.01);
		auto meters = (int)Point::GetDistance(p1, p2);
		ASSERT_EQUAL(meters, 7'886'373);
	}
}

#endif // _DEBUG
//===============================================
// BusStorage ===================================
//-----------------------------------------------
struct BusInfo
{
	bool isCircle;
	vector<string> bus_stops;
	size_t stops_on_route;
	size_t uniq_stops;
};
//-----------------------------------------------
class Request;

class BusManager
{
private:
	unordered_map<string, BusInfo> buses;
	unordered_map<string, Point> bus_stops;
	list<string> responces;

	BusManager(const BusManager&) = delete;
	BusManager(BusManager&&) = delete;

public:
	BusManager() {}
	auto& GetBuses() { return buses; }
	auto& GetBusStops() { return bus_stops; }
	auto& GetResponces() { return responces; }
};
// Request ======================================
enum RequestType
{
	Invalid,
	InputStop,
	InputBus,
	OutputBus
};
//-----------------------------------------------
class Request
{
protected:
	RequestType type;
	string message;
public:
	virtual void Process(BusManager &storage) {};
	Request(const string &str) : type(Invalid), message(str) {}
	virtual ~Request() {}
	RequestType GetType() const { return type; }
	string& GetMessage() { return message; }
};
//-----------------------------------------------
class InputBus_Request : public Request
{
private:
	string bus_number;
	vector<string> stops;
	bool isCircle;
	size_t stops_on_route;
	size_t uniq_stops;
public:
	InputBus_Request(const string &str, string number) : bus_number(move(number)), Request(str)
	{
		type = InputBus;
		auto it = find(message.begin(), message.end(), '-');
		isCircle = (it != message.end());
		char delim = isCircle ? '-' : '>';
		stops = SplitString(message, delim);

		for (auto &stop : stops)
		{
			ClearPrefix(stop, ' ');
			ClearSufix(stop, ' ');
		}

		stops_on_route = isCircle ? (stops.size() * 2) - 1 : stops.size();
		uniq_stops = 0;

		for (auto it = stops.begin(); it != stops.end(); it++)
		{
			if (count(it, stops.end(), *it) == 1)
				uniq_stops++;
		}
	}
	void Process(BusManager &storage) override
	{
		/*
		Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye
		Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka
		*/

		storage.GetBuses()[bus_number] =
		{
			isCircle,
			move(stops),
			stops_on_route,
			uniq_stops,
		};
	}
};
//-----------------------------------------------
class InputStop_Request : public Request
{
	string stop_name;
	Point coord;
public:
	InputStop_Request(const string &str) : Request(str)
	{
		type = InputStop;
		stop_name = GetPrefix(message, ':');
		auto vec = SplitString(message, ',');
		if (vec.size() < 2)
			throw invalid_argument("Request exception");

		coord = Point(stod(vec[0].data()), stod(vec[1].data()));
	}
	void Process(BusManager &storage) override
	{
		storage.GetBusStops()[stop_name] = coord;
	}
};
//-----------------------------------------------
class OutputBus_Request : public Request
{
private:
	string bus_number;
public:
	OutputBus_Request(const string &str, string number) : bus_number(move(number)), Request(str)
	{
		type = OutputBus;
	}
	void Process(BusManager &storage) override
	{
		stringstream stream;
		if (storage.GetBuses().count(bus_number) > 0)
		{
			const auto &bus_info = storage.GetBuses()[bus_number];


			double distance = 0.0;
			Point prev_point;
			double mult = bus_info.isCircle ? 2.0 : 1.0;
			for (size_t i = 0; i < bus_info.bus_stops.size(); i++)
			{
				if (i == 0)
				{
					prev_point = storage.GetBusStops()[bus_info.bus_stops.at(i)];
					continue;
				}

				const Point &curr_point = storage.GetBusStops()[bus_info.bus_stops.at(i)];
				distance += Point::GetDistance(prev_point, curr_point) * mult;
				prev_point = curr_point;
			}
			
			// Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length
			// Bus 750 : 5 stops on route, 3 unique stops, 20939.5 route length
			// Bus 751 : not found
			stream << setprecision(6);
			stream 
				<< "Bus " << bus_number << ": " 
				<< bus_info.stops_on_route << " stops on route, " 
				<< bus_info.uniq_stops << " unique stops, " 
				<< distance << " route length";
		}
		else
		{
			stream << "Bus " << bus_number << ": not found\n";
		}
		message = stream.str();
	}
};
//-----------------------------------------------
unique_ptr<Request> BuildRequest(string str)
{
	if (str.empty())
		return make_unique<Request>("");

	auto first_word = GetPrefix(str);
	if (first_word == "Stop")
	{
		return make_unique<InputStop_Request>(str);
	}
	else
	{
		auto it = find(str.begin(), str.end(), ':');
		if (it == str.end())
		{
			return make_unique<OutputBus_Request>("", string(str));
		}
		else
		{
			auto number = GetPrefix(str, ':');
			return make_unique<InputBus_Request>(str, string(number));
		}
	}
}
// RequestManager ===============================
class RequestManager
{
private:
	istream &input;
	ostream &output;
	BusManager *bus_manager;
	list<string> responces;
	size_t precision;
public:
	RequestManager(istream &_in, ostream &_out)
		: input(_in), output(_out), bus_manager(nullptr), precision(0)
	{}
	void ReadRequests();
	void PrintRecponces() const;
	void SetBusManager(BusManager *_bus_manager) { bus_manager = _bus_manager; }
	void SetPrecision(size_t pr) { precision = pr; }
};
//-----------------------------------------------
void RequestManager::ReadRequests()
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
		//		cerr << buff;
		request->Process(*bus_manager);
	}

	getline(input, buff, '\n');
	count = stoi(buff.c_str());
	for (int i = 0; i < count; i++) // читаем вывод
	{
		getline(input, buff, '\n');
		request = BuildRequest(buff);
		request->Process(*bus_manager);
		responces.push_back(move(request->GetMessage()));
	}
}
//-----------------------------------------------
void RequestManager::PrintRecponces() const
{
	if (precision > 0)
		output << setprecision(precision);
	for (const auto &responce : responces)
		output << responce << '\n';
}
//===============================================
// BuildRequest tests --------------------------
#ifdef _DEBUG
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
//===============================================
//-----------------------------------------------
void Case1()
{
	ifstream in_file(test_folder + "case1_in.txt");
	ifstream expected(test_folder + "case1_out.txt");

	if (!in_file.is_open(), !expected.is_open())
		throw exception("case 1 failed! Can't open files.");
	
	START_PROFILER("Case 1 Profile - ");
	BusManager bus_manager;
	stringstream output;
	RequestManager request_manager(in_file, output);
	request_manager.SetBusManager(&bus_manager);
	request_manager.ReadRequests();
	request_manager.PrintRecponces();
	STOP_PROFILER("Case 1 Profile - ")

	string str_expected, buff;
	while (getline(expected, buff, '\n'))
	{
		str_expected += buff + "\n";
	}
	str_expected.push_back('\n');

	ASSERT_EQUAL(output.str(), str_expected);

}
#endif // _DEBUG
//===============================================


#ifdef _DEBUG

void RunAllTests()
{
	TestRunner tr;
	RUN_TEST(tr, Point_Test);
	RUN_TEST(tr, SpitString_test);
	RUN_TEST(tr, GetPrefix_test);

	RUN_TEST(tr, BuildRequest_test);
	RUN_TEST(tr, ClearPrefixSuffix_test);
	RUN_TEST(tr, Case1);
}
#endif // _DEBUG



int main()
{
#ifdef _DEBUG
	RunAllTests();
#endif // _DEBUG

	BusManager bus_manager;
	stringstream output;
	RequestManager request_manager(cin, cout);
	request_manager.SetBusManager(&bus_manager);
	request_manager.ReadRequests();
	request_manager.PrintRecponces();
}
