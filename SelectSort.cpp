#include <iostream>
using namespace std;

void swap(int* a, int* b)
{
	int tmp = *a;
	*a = *b;
	*b = *a;
}


void SelectSort(int array[], int size)
{
	int end = size - 1;
	for (int i = 0; i < size - 1; i++)
	{
		int max = 0;

		for (int j = 1; j < size - i; j++)
		{
			if (array[j] > array[max])
			{
				max = j;
			}
		}
		swap(array[max], array[end--]);
	}
}

void BubbleSort(int array[], int size)
{
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - i - 1; j++)
		{
			if (array[j] > array[j + 1])
			{
				swap(array[j], array[j + 1]);
			}
		}
	}
}


void SelectSort2(int array[], int size)
{
	int begin = 0;
	int end = size - 1;

	while (begin < end)
	{
		int max = begin;
		int min = begin;
		for (int j = begin; j <= end; j++)
		{
			if (array[max] < array[j])
				max = j;
			if (array[min] > array[j])
				min = j;
		}
		if (min == end)
		{
			swap(array[max], array[end]);
			min = max;
		}
		else
			swap(array[max], array[end]);
		swap(array[min], array[begin]);
		begin++;
		end--;
	}
}


int main()
{
	int array[] = { 0, 33, 1, 23, 42, 13, 100, 61, };
	int size = sizeof(array) / sizeof(array[0]);
	//BubbleSort(array, size);
	SelectSort2(array, size);
	return 0;
}