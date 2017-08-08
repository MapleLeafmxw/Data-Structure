#include <iostream>
using namespace std;

template<class T>
struct Greate
{
	bool operator()(const T& left, const T& right)
	{
		retrun left > right;
	}
};

template<class T>
struct Less
{
	bool operator()(const T& left, const T& right)
	{
		return left < right;
	}
};


template<class T, class Compare>
void ShellSort(T array[], int size)
{
	int gap = size;
	while (gap > 1)
	{
		gap = gap / 3 + 1;
		for (int i = gap; i < size; i++)
		{
			int temp = array[i];
			int end =  i - gap ;
			while (end >= 0 && Compare()(temp, array[end]))
			{
				array[end + gap] = array[end];
				end -= gap;
			}

			array[end+gap] = temp;
		}
	}
}


int main()
{
	int array[] = { 10, 50, 32, 5, 76, 9, 40,/* 88, 20, 33, 13, 19, 2, 24, 11*/};
	int size = sizeof(array) / sizeof(array[0]);
	ShellSort<int, Less<int>>(array, size);
	return 0;
}