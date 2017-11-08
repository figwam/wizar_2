#include "stdafx.h"

/*
	”величение контраста
*/
void contrast(IplImage* src)
{
	float average = 0,
		averageMin = 0,
		countMin = 0,
		averageMax = 0,
		countMax = 0,
		factor;
	for (int i = 0; i < src->height; i++)
	{
		uchar* str = (uchar*)(src->imageData + i * src->widthStep);
		for (int j = 0; j < src->width; j++)
		{
			average += str[j];
		}
	}
	average /= src->height * src->width;
	//Ќахождение максимального и минимального среднего значений €ркости
	for (int i = 0; i < src->height; i++)
	{
		uchar* str = (uchar*)(src->imageData + i * src->widthStep);
		for (int j = 0; j < src->width; j++)
		{
			if (str[j] < average)
			{
				averageMin += str[j];
				++countMin;
			}
			else
			{
				averageMax += str[j];
				++countMax;
			}
		}
	}
	averageMin /= countMin;
	averageMax /= countMax;
	factor = 1 + (255.0 + averageMax + averageMin) / 255.0;
	cout << (255.0 + averageMax + averageMin) / 255.0 << endl;
	for (int i = 0; i < src->height; i++)
	{
		uchar* str = (uchar*)(src->imageData + i * src->widthStep);
		for (int j = 0; j < src->width; j++)
		{
			//смещение влево
			if (averageMin != 0)
			{
				if (str[j] > averageMin)
					str[j] = str[j] - averageMin;
				else
					str[j] = 0;
			}
			if (averageMax != 0)
			{
				if (str[j] < averageMax)
				{
					if (str[j] * factor < 255)
						str[j] *= factor;
					else
						str[j] = 255;
				}
			}
		}
	}
//	cvNamedWindow("w");
//	cvShowImage("w", src);
//	cvWaitKey(0);
}
/*
вернЄт значение пиксел€
*/
short int getPiksel(IplImage* image, int x, int y)
{
	uchar* str = (uchar*)(image->imageData + y * image->widthStep);
	return (short int)str[x];
}
/*
ƒетектор границ
*/
void boundaryDetector(IplImage* src, IplImage* dst)
{
	int xBorder = src->width - 1,
		yBorder = src->height - 1;
	bool status;
	for (short int i = 0; i < src->height; i++)
	{
		uchar* str = (uchar*)(src->imageData + i * src->widthStep);
		uchar* dtr = (uchar*)(dst->imageData + i * dst->widthStep);
		for (short int j = 0; j < src->width; j++)
		{
			dtr[j] = 0;
			if (str[j] > 0)
			{
				status = 0;
				//проверка по вертикали
				if (i != 0 && i != yBorder &&
					(getPiksel(src, j, i + 1) == 0 || getPiksel(src, j, i - 1) == 0))
				{
					status = 1;
				}
				else if (i == 0 && getPiksel(src, j, i + 1) == 0)
					status = 1;

				else if (i == yBorder && getPiksel(src, j, i - 1) == 0)
					status = 1;
				if (status)
				{
					dtr[j] = 255;
					continue;
				}

				//проверка по горизонтали
				if (j != 0 && j != xBorder &&
					(getPiksel(src, j - 1, i) == 0 || getPiksel(src, j + 1, i) == 0))
				{
					status = 1;
				}
				else if (j == 0 && getPiksel(src, j + 1, i) == 0)
					status = 1;

				else if (j == xBorder && getPiksel(src, j - 1, i) == 0)
					status = 1;

				if (status)
					dtr[j] = 255;
			}

		}
	}
}

/*
«акрасит указанную областьв белый
point - координаты области
size - размеры области
color - цвет
*/
void draw(IplImage* img, CvPoint& point, CvSize& size, short int color)
{
	for (short int i = 0; i < size.height; i++)
	{
		uchar* str = (uchar*)(img->imageData + (point.y * size.height + i) * img->widthStep);
		for (short int j = 0; j < size.width; j++)
		{
			str[point.x * size.width + j] = color;
		}
	}
}


/*

*/
void drawCenter(const IplImage* src, const CvPoint& point, const CvSize& size)
{
	IplImage* dst = cvCloneImage(src);
	int X = point.x - size.width / 2,
		Y = point.y - size.height / 2;
	for (int i = Y; i < Y + size.height; i++)
	{
		uchar* dtr = (uchar*)(dst->imageData + i * dst->widthStep);
		for (int j = X; j < X + size.width; j++)
		{
			if (i == point.y && j == point.x)
				dtr[j] = 255;
			else if (dtr[j] <= 100)
				dtr[j] = 100;
		}
	}
	cvNamedWindow("win4");
	cvShowImage("win4", dst);
	cvWaitKey(0);
	cvReleaseImage(&dst);
	cvDestroyWindow("win4");
}

void borders(IplImage* src, IplImage* dst)
{
	const int borderBottom = src->height - 1,//ограничение снизу
		borderRight = src->width - 1;//ограничение справа
	short int delt, deltMax;
	uchar* dtr;
	uchar* str;
	uchar* top;
	uchar* bottom;
	for (int i = 1; i < borderBottom; ++i)
	{
		//	cout << i << "\t" << borderBottom << endl;

		dtr = (uchar*)(dst->imageData + i * dst->widthStep);
		str = (uchar*)(src->imageData + i * src->widthStep);
		top = (uchar*)(src->imageData + (i - 1) * src->widthStep);
		bottom = (uchar*)(src->imageData + (i + 1) * src->widthStep);
		for (int j = 1; j < borderRight; ++j)
		{
			deltMax = abs(str[j - 1] - str[j + 1]);
			for (short int c = -1; c < 2; c++)
			{
				delt = abs(top[j - c] - bottom[j + c]);
				if (delt > deltMax)
					deltMax = delt;
			}
			dtr[j] = deltMax;

		}
	}
	/*
		cvNamedWindow("grayTest");
	cvShowImage("grayTest", src);
	cvNamedWindow("neuron 1 layer");
	cvShowImage("neuron 1 layer", dst);
	cvWaitKey(0);
	cvDestroyWindow("neuron 1 layer");
	cvReleaseImage(&dst);
	*/
}

void ini(IplImage* image, short int val)
{
	for (int i = 0; i < image->height; i++)
	{
		uchar* str = (uchar*)(image->imageData + i * image->widthStep);
		for (int j = 0; j < image->width; j++)
		{
			str[j] = val;
		}
	}
}

/*
ѕроизводит группировку белых пикселей
вернЄт список всех групп
*/
list<Form>& group(IplImage* bin, list<Form>& repository, const int radius, const int filter)
{
	const int borderBottom = bin->height - radius,
		borderRifht = bin->width - radius;
	int winTop,
		winBottom,
		winLeft,
		winRight;
	uchar* str;
	
	queue<Point> pik;
	for (int i = radius; i < bin->height; ++i)
	{
		str = (uchar*)(bin->imageData + i * bin->widthStep);
		for (int j = radius; j < bin->width; ++j)
		{
			//поиск белого пиксел€
			if (str[j] == 255)
			{
				str[j] = 123;
				Form form;
				Point point(j, i);
				form.add(point);
				pik.push(point);

				while (!pik.empty())
				{
					//”становка геометрических размеров окна вокруг белого пиксел€
					winTop = pik.front().y - radius;
					winBottom = pik.front().y + radius - 1;
					winLeft = pik.front().x - radius;
					winRight = pik.front().x + radius - 1;

					for (int a = winTop; a < winBottom; ++a)
					{
						
						uchar*  win = (uchar*)(bin->imageData + a * bin->widthStep);
						for (int b = winLeft; b < winRight; ++b)
						{
							//поиск других белых пикселей
							if (win[b] == 255)
							{
								win[b] = 123;
								point.x = b;
								point.y = a;
								form.add(point);
								if(a >= radius && a < borderBottom && b >= radius && b < borderRifht)
									pik.push(point);
							}
						}
					}
					//удаление из очереди пиксел€, окрестности которого проверены
					pik.pop();
				}
				//ƒобавление группы в список
				repository.push_back(form);
			}
		}
	}
	list<Form>::iterator iter = repository.begin();
	while (iter != repository.end())
	{
		if (iter->size <= filter)
			iter = repository.erase(iter);
		else
			++iter;
	}


	return repository;
}

/*
	ѕоказывает список образов на чЄрном фону=е
*/
void viewForm(list<Form> forms, IplImage* src)
{
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	for (int i = 0; i < dst->height; i++)
	{
		uchar* str = (uchar*)(dst->imageData + i * dst->widthStep);
		for (int j = 0; j < dst->width; j++)
		{
			str[j*3] = 0;
			str[j*3 + 1] = 0;
			str[j*3 + 2] = 0;
		}
	}
	list<Form>::const_iterator iter = forms.begin();
	uchar* dtr;
	uchar* str;
	while (iter != forms.end())
	{
		for (int i = iter->minY; i < iter->maxY; i++)
		{
			str = (uchar*)(src->imageData + i * src->widthStep);
			dtr = (uchar*)(dst->imageData + i * dst->widthStep);
			for (int j = iter->minX; j < iter->maxX; j++)
			{
				dtr[j * 3] = str[j * 3];
				dtr[j * 3 + 1] = str[j * 3 + 1];
				dtr[j * 3 + 2] = str[j * 3 + 2];
			}
		}
		++iter;
	}
	cvNamedWindow("dst");
	cvNamedWindow("src");
	cvShowImage("dst", dst);
	cvShowImage("src", src);
	cvWaitKey(0);
	cvDestroyWindow("dst");
	cvDestroyWindow("src");
	cvReleaseImage(&dst);
}