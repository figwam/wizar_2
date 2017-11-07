#include "stdafx.h"
#include "point.h"

Point::Point()
{}
Point::Point(const int x, const int y)
{
	this->x = x;
	this->y = y;
}
Point::Point(const Point& point)
{
	this->x = point.x;
	this->y = point.y;
}
bool Point::operator==(const Point& right)const
{
	return (this->x == right.x && this->y == right.y) ? 1 : 0;
}
bool Point::operator< (const Point& right)const
{
	if (this->y < right.y)
		return 1;
	else if (this->y > right.y)
		return 0;
	if (this->x < right.x)
		return 1;
	return 0;
}