#pragma once
#include "stdafx.h";
#include <list>
#include "body/header/point.h"
#include "body/header/neural/lauer.h"

using namespace std;
/*
	x - ����������� ��������
	y - ���������� ��������
*/
template<short int x, short int y>
class neuron_1
{
protected:
	float* weight = new float[x*y];	//������� ������������
	float positiveWeight, negativeWeight;//����� ������������� � ������������� ������������� (�� �������� �������)
	short int*	vectors = new short int[x * y];	//������ �������� (��� ����������� �������)
	short int* length = new short int[y];		//������ ���������� �������� � ������ ������ (��� vectors)
	short int piksels = 0;	//���������� ���� ������� ���������� �������� (��� vectors)
	
	short int next(short int i)
	{
		return x*i + length[i];
	}
	/*
	������������ ������� ������������� �������,
	�� ���� ���������� �������� ������������ � ����������� ���������� �� �������
	*/
	void rangingDistance()
	{
		float minDistance,
			distance;
		short int position;
		//������� ������� �������������
		for (short int i = 0; i < y; ++i)
		{
			for (short int j = 0; j < x; ++j)
			{
				position = i * x + j;
				minDistance = 15;
				if (weight[position] != 2)
				{
					//������� ����������� �������� � ���������� ����������
					for (short int a = 0; a < y; ++a)
					{
						for (short int b = 0; b < length[a]; b++)
						{
							distance = sqrt(abs(i - a) + abs(j - vectors[a * x + b]));
							if (distance < minDistance)
								minDistance = distance;
						}
					}
					weight[position] /= minDistance * 1.8;
					if (weight[position] > 0)
						positiveWeight += weight[position];
					else
						negativeWeight += weight[position];
				}
			}
		}

	}
public:	
	~neuron_1()
	{
		delete[] weight;
		delete[] vectors;
		delete[] length;
	}
	neuron_1()
	{
		for (short int i = 0; i < y; i++)
		{
			length[i] = 0;
			for (short int j = 0; j < x; j++)
				vectors[x * i + j] = 0;
		}
		piksels = 0;
	}
	neuron_1(const short int* arr)
	{
		this->add(arr);
	}
	//����������� �����������
	neuron_1(const neuron_1<x, y>& right)
	{
		this->piksels = right.piksels;
		this->positiveWeight = right.positiveWeight;
		this->negativeWeight = right.negativeWeight;
		int size = x * y;
		for (short int i = 0; i < y; i++)
		{
			this->length[i] = right.length[i];
		}
		for (int i = 0; i < size; i++)
		{
			weight[i] = right.weight[i];
			vectors[i] = right.vectors[i];
		}
	}
	/*
	�������� ������������
	*/
	neuron_1<x, y>& operator=(const neuron_1<x, y>& right)
	{
		this->piksels = right.piksels;
		this->positiveWeight = right.positiveWeight;
		this->negativeWeight = right.negativeWeight;
		int size = x * y;
		for (short int i = 0; i < y; i++)
		{
			this->length[i] = right.length[i];
		}
		for (int i = 0; i < size; i++)
		{
			weight[i] = right.weight[i];
			vectors[i] = right.vectors[i];
		}
		return *this;
	}
	/*
	������� ��������� (��������� �� vectors)
	*/
	bool operator==(const neuron_1<x, y>& right) const
	{
		if (piksels != right.piksels)
			return false;
		for (short int i = 0; i < y; i++)
		{
			if (this->length[i] != right.length[i])
				return false;
			for (short int j = 0; j < length[i]; j++)
				if (this->vectors[x * i + j] != right.vectors[x * i + j])
					return false;
		}
		return true;
	}
	/*
	��������� ��������� (��������� �� vectors)
	*/
	float compare(const neuron_1<x, y>& right) const
	{
		short int lenVec,//���������� ����� �������
			position;//������� �������
		float one = 1.0,//�������������
			oneWeight; //��� ������ �������

		if (piksels > right.piksels)
			oneWeight = one / piksels;
		else
			oneWeight = one / right.piksels;

		for (short int i = 0; i < y; i++)
		{
			//���� � ������ �������� ���
			if (!length[i] && !right.length[i])
				continue;

			//����� ���������� ������
			if (length[i] >= right.length[i])
				lenVec = length[i];
			else
				lenVec = right.length[i];

			//��������� �� ���������� ������
			for (short int j = 0; j < lenVec; j++)
			{
				position = x * i + j;
				if (vectors[position] != right.vectors[position])
					one -= oneWeight;
			}
		}
		return one;
	}
	//����� ������� � ������� (��� �������)
	void print_r() const
	{
		cout << endl << "================================================" << endl;
		for (short int i = 0; i < y; i++)
		{
			for (short int j = 0; j < x; j++)
				cout << vectors[x * i + j] << "  ";
			cout << endl << endl;
		}
		cout << endl << "================================================" << endl;
	}
	//������������ ������� (��� �������), � ����������� �20
	void viewBorder() const
	{
		short int n = 20;
		CvPoint point;
		CvSize sizeImg, size;
		size.height = n;
		size.width = n;

		sizeImg.height = n * y;
		sizeImg.width = n * x;
		IplImage* img = cvCreateImage(sizeImg, IPL_DEPTH_8U, 1);

		for (short int i = 0; i < sizeImg.height; i++)
		{
			uchar* str = (uchar*)(img->imageData + i * img->widthStep);
			for (short int j = 0; j < sizeImg.width; j++)
				str[j] = 0;
		}

		for (short int i = 0; i < y; i++)
		{
			point.y = i;
			for (int j = 0; j < length[i]; j++)
			{
				point.x = vectors[x * i + j];
				draw(img, point, size, 255);
			}
		}
		cvNamedWindow("neuron 1 layer");
		cvShowImage("neuron 1 layer", img);
		cvWaitKey(0);
		cvDestroyWindow("neuron 1 layer");
		cvReleaseImage(&img);
	}
	/*
		������������ ������� ������������� ������� (��� �������), � ����������� �20
		��� ���� �������, ��� ���� �����������
	*/
	void viewWeight() const
	{
		short int n = 20,
			border = 255;
		IplImage* img = cvCreateImage(CvSize(x * n, y * n), IPL_DEPTH_8U, 1);
	
		for (short int i = 0; i < y; i++)
		{
			for (short int j = 0; j < x; j++)
			{
				for (short int a = 0; a < n; a++)
				{
					uchar* str = (uchar*)(img->imageData + (i * n + a) * img->widthStep);
					for (short int b = 0; b < n; b++)
					{
						if (weight[i * x + j] > 0)
							str[j * n + b] = weight[i * x + j] * 122;

						else
							str[j * n + b] = weight[i * x + j] * 255 / 2;
					}
				}
			}
		}
		cvNamedWindow("neuron 1 layer");
		cvShowImage("neuron 1 layer", img);
		cvWaitKey(0);
		cvDestroyWindow("neuron 1 layer");
		cvReleaseImage(&img);
	}
	//����� 
	Point& getPoint(short int numb) const
	{
		if (numb > piksels)
			return Point(-1, -1);
		short int i;
		for (i = 0; numb >= length[i]; i++)
			numb -= length[i];
		return Point(vectors[i * x + numb],i);
	}
	/*
	�������� ������� �� ���������� ����� �����������
	imgX - ���������� ���� �� ��� �
	imgY - ���������� ���� �� ��� y
	x - ������ ����
	y - ������ ����
	neuron - ��������� ������ ��� �������
	*/
	static void createNeuron(const IplImage* bin, const IplImage* img, neuron_1<x, y>& neuron, short int imgX, short int imgY)
	{
		int xBorder = imgX + x - 1,
			yBorder = imgY + y - 1;
		bool status;
		for (short int i = 0; i < y; i++)
		{
			neuron.length[i] = 0;
			uchar* str = (uchar*)(img->imageData + (imgY + i) * img->widthStep);
			uchar* bnt = (uchar*)(bin->imageData + (imgY + i) * bin->widthStep);
			for (short int j = 0; j < x; j++)
			{
				neuron.vectors[x * i + j] = -1;
				if (bnt[imgX + j] > 0)
				{
					if (str[imgX + j] > 0)
					{
						neuron.weight[x * i + j] = 2;
						neuron.vectors[neuron.next(i)] = j;
						++neuron.length[i];
						++neuron.piksels;
					}
					else
					{
						neuron.weight[x * i + j] = 1;
					}
				}
				else
				{
					neuron.weight[x * i + j] = -1;
				}
			}
		}
		//���������� ���������� �� ������� ������������ �������

	}
		
	/*
	���������� ������� (��� �����������)
	image - ��������� �����������
	repository - ��������� �����������
	*/
	static void train(IplImage* bin, list<neuron_1<x, y>>& repository)
	{
		IplImage* image = cvCreateImage(cvGetSize(bin), IPL_DEPTH_8U, 1);;
		boundaryDetector(bin, image);

		const int xx = x, yy = y;
		int size = x,
			half = size / 2,
			part = 1,
			borderX = image->width - size,
			borderY = (image->height) - size;

		uchar* check;
		for (int a = 0; a < borderY; a += part)
		{
			check = (uchar*)(image->imageData + (a + half) * image->widthStep);
			for (int b = 0; b < borderX; b += part)
			{
				//�������� ������������ �������
				if (check[b + half] < 123)
					continue;
				neuron_1<xx, yy>* neuron = new neuron_1<xx, yy>;
				createNeuron(bin, image, *neuron, b, a);
				if (!checkNeuro_1(repository, *neuron))
				{
					neuron->rangingDistance();
					repository.push_back(*neuron);
					delete neuron;
				}

			}
		}
	}

	//�����
	static bool checkNeuro_1(const list<neuron_1<x, y>>& repository, const neuron_1<x, y>& neuron)
	{
		const int xx = x, yy = y;
		list<neuron_1<xx, yy>>::const_iterator iter = repository.begin();
		while (iter != repository.end())
		{
			if (*iter == neuron)
				return 1;
			++iter;
		}
		return 0;
	}

	/*
		�������� ������
		image - �����-�����, ������������� �����������
		lauer - ������ ������������ ���� "Lauer"
		dst - �����������, ���� ����� ������� ���������
	*/
	static void borderDetector(IplImage* image, Lauer<neuron_1<x, y> >& lauer, IplImage* dst)
	{
		const short int delt = 25;
		const int sizeLauer = lauer.length(),
			yHalf = y / 2,
			xHalf = x / 2,
			borderTop = yHalf, //����������� ������
			borderBottom = image->height - borderTop,//����������� �����
			borderLeft = xHalf,//����������� �����
			borderRight = image->width - borderLeft;//����������� ������
		int	aMin, aMax, bMin, bMax, //����������� �� ������� �������
			counter;
		const float size = x * y;
		float average,//������� ������� ��������
			minAverage, minCount, //���������� ������� ������� �������� � �� �������
			maxAverage, maxCount, //������������ ������� ������� �������� � �� �������
			negative, positive, borderWeight,
			case_1, case_2,
			per = 0.96; //�������������
		float* weig;

		uchar* str;
		uchar* dtr;
		for (int i = borderTop; i < borderBottom; ++i)
		{
			dtr = (uchar*)(dst->imageData + i * dst->widthStep);
			cout << i << "\t"<< borderBottom << endl;
			for (int j = borderLeft; j < borderRight; ++j)
			{
				average = 0.0;
				aMin = i - yHalf;
				aMax = aMin + y;
				bMin = j - xHalf;
				bMax = bMin + x;
				//���������� ������� �������
				for (short int a = aMin; a < aMax; ++a)
				{
					str = (uchar*)(image->imageData + a * image->widthStep);
					for (short int b = bMin; b < bMax; ++b)
					{
						average += str[b];
					}
				}
				average /= size;
				if (average < delt || average > 255 - delt)
					continue;
				minAverage = 0,0;
				minCount = 0;
				maxAverage = 0.0;
				maxCount = 0;
				//���������� ������������ � ������������� �������� �������� �������
				for (short int a = aMin; a < aMax; ++a)
				{
					str = (uchar*)(image->imageData + a * image->widthStep);
					for (short int b = bMin; b < bMax; ++b)
					{
						if (str[b] >= average)
						{
							maxAverage += str[b];
							++maxCount;
						}
						else
						{
							minAverage += str[b];
							++minCount;
						}
					}
				}
				maxAverage /= maxCount;
				minAverage /= minCount;
				if (maxAverage - minAverage < delt)
					continue;
				//���������� ��� ������� ���������� �������� �������
				for (int c = 0; c < sizeLauer; ++c)
				{
					negative = 0;
					positive = 0;
					borderWeight = 0;
					counter = 0;
					weig = lauer[c]->weight;
					//������ ������: ������������� ���� ������ �� ����� ����� �����������
					case_1 = (lauer[c]->negativeWeight - lauer[c]->piksels * 2) * maxAverage + lauer[c]->positiveWeight * minAverage;

					//������ ������: ������������� ���� ������ �� ����� ����� �����������
					case_2 = (lauer[c]->positiveWeight + lauer[c]->piksels * 2) * maxAverage + lauer[c]->negativeWeight * minAverage;
					//��������� �������� ��������
					for (short int a = aMin; a < aMax; ++a)
					{
						str = (uchar*)(image->imageData + a * image->widthStep);
						for (short int b = bMin; b < bMax; ++b)
						{
							//��� ������������� ������� �������������
							if (weig[counter] < 0)
								negative += weig[counter] * str[b];
							//��� �������
							else if (weig[counter] == 2)
								borderWeight += weig[counter] * str[b];
							//��� ������������� ������� �������������
							else
								positive += weig[counter] * str[b];
							++counter;
						}
					}
					
					if ((negative - borderWeight + positive) <= case_1 * per)
					{
						dtr[j] = 255;
						break;
					}
					if ((positive + borderWeight + negative) >= case_2 * per)
					{
						dtr[j] = 255;
						break;
					}
				}
			}
		}
		cvNamedWindow("grayTest");
		cvShowImage("grayTest", image);
		cvNamedWindow("neuron 1 layer");
		cvShowImage("neuron 1 layer", dst);
		cvWaitKey(0);
		cvDestroyWindow("neuron 1 layer");
		cvReleaseImage(&image);
	}

};