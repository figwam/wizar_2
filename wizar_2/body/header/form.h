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
	����� ����������� �� maxY, maxX, minY, minX
	����� 1, ���� ������ ������������
	*/
	bool intersect(const Form&);
	/*
		����������� �������
	*/
	void combine(const Form&);
	//������������
	void view();

	void inflict(IplImage*, const Form&);
};
