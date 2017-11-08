// wizar_2.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"


int main()
{
	//������ ��������
	const short int n = 15;
	//��������� �������� ����������� 
	IplImage* image = cvLoadImage("C:\\Users\\figwam\\Desktop\\photo\\1.png");

	IplImage* gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	cvCvtColor(image, gray, CV_RGB2GRAY);

	list<neuron_1<n, n>> repository;
	//���������� ��������
	neuron_1<n, n>::train(gray, repository);
	
	//�������� �����������
	IplImage* test = cvLoadImage("C:\\Users\\figwam\\Desktop\\photo\\9.jpg");
	IplImage* grayTest = cvCreateImage(cvGetSize(test), IPL_DEPTH_8U, 1);
	//� �������� ������
	cvCvtColor(test, grayTest, CV_RGB2GRAY);
//	contrast(grayTest);

	Lauer<neuron_1<n, n> > lauer(repository);
	IplImage* dst_2 = cvCreateImage(cvGetSize(grayTest), IPL_DEPTH_8U, 1);
	ini(dst_2);
	//�������������� ������
	neuron_1<n, n>::borderDetector(grayTest, lauer, dst_2);

	IplImage* first = cvCloneImage(dst_2);
	list<Form> forms;
	//�����������
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

	//�����������
	viewForm(forms, test);
    return 0;
}

