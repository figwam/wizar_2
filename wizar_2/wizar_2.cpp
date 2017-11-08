// wizar_2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	//размер нейронов
	const short int n = 15;
	//обучающее бинарное изображение 
	IplImage* image = cvLoadImage("C:\\Users\\figwam\\Desktop\\photo\\1.png");

	IplImage* gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	cvCvtColor(image, gray, CV_RGB2GRAY);

	list<neuron_1<n, n>> repository;
	//тренировка нейронов
	neuron_1<n, n>::train(gray, repository);
	
	//тестовое изображение
	IplImage* test = cvLoadImage("C:\\Users\\figwam\\Desktop\\photo\\9.jpg");
	IplImage* grayTest = cvCreateImage(cvGetSize(test), IPL_DEPTH_8U, 1);
	//в градации серого
	cvCvtColor(test, grayTest, CV_RGB2GRAY);
//	contrast(grayTest);

	Lauer<neuron_1<n, n> > lauer(repository);
	IplImage* dst_2 = cvCreateImage(cvGetSize(grayTest), IPL_DEPTH_8U, 1);
	ini(dst_2);
	//детектирование границ
	neuron_1<n, n>::borderDetector(grayTest, lauer, dst_2);

	IplImage* first = cvCloneImage(dst_2);
	list<Form> forms;
	//группировка
	group(dst_2, forms, 1);
	list<Form>::iterator iter = forms.begin();
	while (iter != forms.end())
	{
		if (iter->size <= 10)
		{
			iter = forms.erase(iter);
		}
		else
		{

			++iter;
		}
	}

	//отображение
	viewForm(forms, test);
    return 0;
}

