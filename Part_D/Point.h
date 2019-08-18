#pragma once
#include <memory>
#include <math.h>
#include <iostream>

constexpr const double PI = 3.1415926535;
constexpr const double EARTH_RAD = 6371000.0;

class Point
{
private:
	double
		lat, // широта в радинах
		lon; // долгота в радианах

	static std::pair<double, double> RadiansFromAngles(double lat, double lon);
public:
	Point() : lat(0.0), lon(0.0) {}
	Point(double _lat, double _lon);
	Point(const Point& other) = default;
	bool IsNull() const { return lat == 0.0 && lon == 0.0; }

	const Point& operator = (const Point &p);

	std::pair<double, double> GetCoord() const; // {lat, lon}
	static double GetDistance(const Point& l, const Point& r);
	bool operator == (const Point& p2) const;
	friend std::ostream& operator << (std::ostream& out, const Point& p);
};
// Point Tests ==================================
#ifdef _DEBUG
void Point_Test();
#endif // _DEBUG