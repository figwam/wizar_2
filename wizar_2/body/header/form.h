#pragma once
#include "stdafx.h"
#include "point.h"
#include <list>
using namespace std;

struct Form
{
	list<Point> piksels;
	int maxY, maxX, minY, minX, size;
	Form();
	void add(Point);
	/*
	Поиск пересечения по maxY, maxX, minY, minX
	Вернёт 1, если образы пересекаются
	*/
	bool intersect(const Form&);
	/*
		Объединение образов
	*/
	void combine(const Form&);
	//визуализация
	void view();

	void inflict(IplImage*, const Form&);
};
