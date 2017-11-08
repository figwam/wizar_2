#pragma once
#include "stdafx.h"
#include "body/header/form.h"
inline bool compare(const CvPoint& left, const CvPoint& right)
{
	return (left.x == right.x && left.y == right.y) ? 1 : 0;
}
void contrast(IplImage*);
short int getPiksel(IplImage*, int, int);
//Детектор границ
void boundaryDetector(IplImage*, IplImage*);
void draw(IplImage*, CvPoint&, CvSize&, short int);
//
void drawCenter(const IplImage*, const CvPoint&, const CvSize&);
void borders(IplImage*, IplImage*);
void ini(IplImage* image, short int val = 0);

/*
	Производит группировку белых пикселей
	вернёт список всех групп
*/
list<Form>& group(IplImage*, list<Form>&, const int, const int filter = 10);

void viewForm(list<Form>, IplImage*);