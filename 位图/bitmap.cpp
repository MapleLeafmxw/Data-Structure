#pragma once;
#include <iostream>
using namespace std;
#include <vector>


class Bitmap
{
public:
	Bitmap()
		:size(0)
	{}

	Bitmap(size_t size)
		:size(0)
	{
		arr.resize((size >> 5) + 1);
	}

	void Set(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (arr[index] & (1 << i))
			return;
		else
		{
			arr[index] |= (1 << i);
			size++;
		}
	}
	void ReSet(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (arr[index] & (1 << i))
		{
			arr[index] &= ~(1 << i);
			--size;
		}
		else
			return;
	}
	
	bool test(size_t num)
	{
		size_t index = num >> 5;
		size_t i = num % 32;
		if (arr[index] & (1 << i))
			return true;
		else
			return false;
	}

private:
	vector<int> arr;
	size_t size;
};

