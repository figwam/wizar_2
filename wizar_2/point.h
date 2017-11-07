#pragma once
#include "stdafx.h"

struct Point
{
	int x;
	int y;
	Point();
	Point(const int, const int);
	Point(const Point&);
	bool operator==(const Point&) const;
	bool operator< (const Point&) const;
};