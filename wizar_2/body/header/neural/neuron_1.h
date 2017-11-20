#pragma once
#include "stdafx.h";
#include <list>
#include "body/header/point.h"
#include "body/header/neural/lauer.h"

using namespace std;
/*
	x - вместимость векторов
	y - количество векторов
*/
template<short int x, short int y>
class neuron_1
{
protected:
	float* weight = new float[x*y];	//весовые коэффициенты
	float positiveWeight, negativeWeight;//суммы положительных и отрицательных коэффициентов (не учитывая границы)
	short int*	vectors = new short int[x * y];	//вектор векторов (все пограничные пиксели)
	short int* length = new short int[y];		//хранит количество пикселей в каждой строке (для vectors)
	short int piksels = 0;	//количество всех несущих информацию пикселей (для vectors)
	
	short int next(short int i)
	{
		return x*i + length[i];
	}
	/*
	Ранжирование весовых коэффициентов нейрона,
	по типу уменьшения весового коэффициента с увеличением расстояния от границы
	*/
	void rangingDistance()
	{
		float minDistance,
			distance;
		short int position;
		//перебор весовых коэффициентов
		for (short int i = 0; i < y; ++i)
		{
			for (short int j = 0; j < x; ++j)
			{
				position = i * x + j;
				minDistance = 15;
				if (weight[position] != 2)
				{
					//перебор пограничных пикселей и нахождение расстояний
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
	//конструктор копирования
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
	Оператор присваивания
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
	Строгое сравнение (сравнение по vectors)
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
	Нестрогое сравнение (сравнение по vectors)
	*/
	float compare(const neuron_1<x, y>& right) const
	{
		short int lenVec,//наибольшая длина вектора
			position;//позиция пикселя
		float one = 1.0,//совместимость
			oneWeight; //вес одного пикселя

		if (piksels > right.piksels)
			oneWeight = one / piksels;
		else
			oneWeight = one / right.piksels;

		for (short int i = 0; i < y; i++)
		{
			//если в строке пикселей нет
			if (!length[i] && !right.length[i])
				continue;

			//поиск наибольшей строки
			if (length[i] >= right.length[i])
				lenVec = length[i];
			else
				lenVec = right.length[i];

			//сравнение по наибольшей строке
			for (short int j = 0; j < lenVec; j++)
			{
				position = x * i + j;
				if (vectors[position] != right.vectors[position])
					one -= oneWeight;
			}
		}
		return one;
	}
	//вывод нейрона в консоль (для отладки)
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
	//визуализация нейрона (для отладки), с увеличением х20
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
		Визуализация весовых коэффициентов нейрона (для отладки), с увеличением х20
		Чем ярче квадрат, тем выше коэффициент
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
	//Вернёт 
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
	создание нейрона по переданной части изображения
	imgX - координата окна по оси х
	imgY - координата окна по оси y
	x - ширина окна
	y - высота окна
	neuron - выделеная память для нейрона
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
		//рассчитать расстояние до каждого пограничного пикселя

	}
		
	/*
	тренировка нейрона (для квадратного)
	image - обучающее изображение
	repository - хранилище изображений
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
				//проверка центрального пикселя
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

	//Поиск
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
		Детектор границ
		image - чёрно-белое, детектируемое изображение
		lauer - массив персептронов типа "Lauer"
		dst - изображение, куда будет помещён результат
	*/
	static void borderDetector(IplImage* image, Lauer<neuron_1<x, y> >& lauer, IplImage* dst)
	{
		const short int delt = 25;
		const int sizeLauer = lauer.length(),
			yHalf = y / 2,
			xHalf = x / 2,
			borderTop = yHalf, //ограничение сверху
			borderBottom = image->height - borderTop,//ограничение снизу
			borderLeft = xHalf,//ограничение слува
			borderRight = image->width - borderLeft;//ограничение справа
		int	aMin, aMax, bMin, bMax, //ограничение по размеру нейрона
			counter;
		const float size = x * y;
		float average,//среднее яркости пикселей
			minAverage, minCount, //минмальное среднее яркости пикселей и их счётчик
			maxAverage, maxCount, //максимальное среднее яркости пикселей и их счётчик
			negative, positive, borderWeight,
			case_1, case_2,
			per = 0.96; //достоверность
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
				//нахождение средней яркости
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
				//Нахождение минимального и максимального среднего значения яркости
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
				//Нахождение для каждого прсептрона величины отклика
				for (int c = 0; c < sizeLauer; ++c)
				{
					negative = 0;
					positive = 0;
					borderWeight = 0;
					counter = 0;
					weig = lauer[c]->weight;
					//первый случай: отрицательные веса попали на яркую часть изображения
					case_1 = (lauer[c]->negativeWeight - lauer[c]->piksels * 2) * maxAverage + lauer[c]->positiveWeight * minAverage;

					//второй случай: положительные веса попали на яркую часть изображения
					case_2 = (lauer[c]->positiveWeight + lauer[c]->piksels * 2) * maxAverage + lauer[c]->negativeWeight * minAverage;
					//ожидаемые значения откликов
					for (short int a = aMin; a < aMax; ++a)
					{
						str = (uchar*)(image->imageData + a * image->widthStep);
						for (short int b = bMin; b < bMax; ++b)
						{
							//для отрицательных весовых коэффициентов
							if (weig[counter] < 0)
								negative += weig[counter] * str[b];
							//для границы
							else if (weig[counter] == 2)
								borderWeight += weig[counter] * str[b];
							//для положительных весовых коэффициентов
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