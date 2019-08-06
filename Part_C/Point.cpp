#include "Point.h"

using namespace std;

#ifdef _DEBUG
#include <test_runner.h>

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
	auto[lat_rad, lon_rad] = RadiansFromAngles(_lat, _lon);
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