#pragma once
#include "stdafx.h";
#include <list>
#include "point.h"

using namespace std;

template<class T>
struct Lauer
{
private:
	T** vector;
	unsigned int size = 0;
public:
	/*
	Преобразует список нейронов в одномерный динамический массив адресов нейронов
	*/
	Lauer(list<T>& repository)
	{
		vector = new T*[repository.size()];
		list<T>::iterator iter = repository.begin();
		int counter = 0;
		while (iter != repository.end())
		{
			vector[counter] = &(*iter);
			++counter;
			++iter;
		}
		this->size = counter;
	}
	//длина массива
	unsigned int length()
	{
		return this->size;
	}
	T* operator[] (unsigned int numb)
	{
		return vector[numb];
	}
	const T* operator[] (unsigned int numb) const
	{
		return vector[numb];
	}
	~Lauer()
	{
		delete[] vector;
	}
};