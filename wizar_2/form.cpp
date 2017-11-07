#include "stdafx.h"
#include "form.h"

Form::Form() : maxY(-1), maxX(-1), minY(-1), minX(-1), size(0){}
void Form::add(Point point)
{
	if (point.y > 380)
	{
		cout << "point.y = " << point.y << endl;
		system("pause");
	}
	if (maxY < 0)
	{
		maxX = point.x;
		minX = point.x;
		maxY = point.y;
		minY = point.y;
	}
	if (point.x > maxX)
		maxX = point.x;
	else if (point.x < minX)
		minX = point.x;
	
	if (point.y > maxY)
		maxY = point.y;
	else if (point.y < minY)
		minY = point.y;
	this->piksels.push_back(point);
	++size;
}
bool Form::intersect(const Form& form)
{
	if((form.maxY > maxY > form.minY || maxY > form.maxY > minY ||
		form.minY < minY < form.maxY || minY < form.minY < maxY)
									 &&
		(form.maxX > maxX > form.minX || maxX > form.maxX > minX ||
		form.minX < minX < form.maxX || minX < form.minX < maxX))
	{
		return 1;
	}
	return 0;
}

void Form::combine(const Form& form)
{
	this->piksels.insert(this->piksels.end(), form.piksels.begin(), form.piksels.end());

	if (form.maxX > maxX)
		maxX = form.maxX;
	else if (form.minX < minX)
		minX = form.minX;

	if (form.maxY > maxY)
		maxY = form.maxY;
	else if (form.minY < minY)
		minY = form.minY;

	size += form.size;
}

void Form::view()
{
	{
		const int n = 5,
			sizeX = maxY * n,
			sizeY = maxX * n;
		int ym, xm;
		IplImage* image = cvCreateImage(CvSize(sizeY, sizeX), IPL_DEPTH_8U, 1);
		ini(image);
		list<Point>::const_iterator iter = piksels.begin();

		while (iter != piksels.end())
		{
			ym = iter->y * n;
			xm = iter->x * n;
			for (int i = ym; i < ym + n; ++i)
			{
				uchar* str = (uchar*)(image->imageData + i * image->widthStep);
				for (int j = xm; j < xm + n; j++)
				{
					str[j] = 255;
				}
			}
			++iter;
		}
		cvNamedWindow("view Form");
		cvShowImage("view Form", image);
		cvWaitKey(0);
		cvDestroyWindow("view Form");
		cvReleaseImage(&image);
	}
}