#include "iostream"
using namespace std;
#include <vector>
#include <assert.h>

template<class T>
struct Less
{
	bool operator()(const T& left, const T& right)
	{
		return left < right;
	}
};

template<class T>
struct Greater
{
	bool operator()(const T& left, const T& right)
	{
		return left > right;
	}
};

// 小堆
template<class T, class Compare = Less<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T array[], int size)
	{
		_heap.reserve(size);
		for (int i = 0; i < size; ++i)
			_heap.push_back(array[i]);
		int index = (_heap.size() - 1)>>1;
		for (; 0 <= index; --index)
			_AdjustDown(index);
	}
	void Insert(const T& data)
	{
		_heap.push_back(data);
		int index = (_heap.size() - 1) >> 1;
		//堆的插入每次都在已经建成的而最小堆的后面插入，但插入之后，有可能破坏了堆的结构，这时就需要对堆自下而上的对其调整。
		_AdjustUp(index);
	}
	
	void Remove()
	{
		assert(!_heap.empty());
		std::swap(_heap[0], _heap[_heap.size() - 1]);
		_heap.pop_back();
		//堆的删除是：从堆中删除堆顶元素。移除堆顶元素之后，用堆的最后一个节点填补取 走的堆顶元素，并将堆的实际元素个数减1。
		//但用最后一个元素取代堆顶元素之后有可能破坏堆，因此需要将对自顶向下调整，使其满足最大或最小堆。
		_AdjustDown(0);
	}

	size_t Size()const
	{
		return _heap.size();
	}

	bool Empty()const
	{
		return _heap.empty();
	}
	
	const T& Top()const
	{
		return _heap[0];
	}

protected:
	void _AdjustDown(size_t parent)
	{
		size_t child = parent * 2 + 1;
		while (child < _heap.size())
		{
			Compare com;
			if ((child + 1) < _heap.size() && com(_heap[child + 1], _heap[child]))
				child += 1;
			if (com(_heap[child], _heap[parent]))
			{
				std::swap(_heap[parent], _heap[child]);
				parent = child;
				child = child * 2 +1;
			}
			else
				return;
		}
	}
	
	void _AdjustUp(size_t child)
	{
		size_t parent = (child - 1)>>1;
		while (child != 0)
		{
			if (Compare()(_heap[child], _heap[parent]))
			{
				std::swap(_heap[child], _heap[parent]);
				child = parent;
				parent = (child - 1) >> 1;;
			}
			else
				return;
		}
	}
private:
	std::vector<T> _heap;
};

template<class T, class Compare = Less<T>>
class PriorityQueue
{
public:
	PriorityQueue()
	{}

	void Push(const T& data)
	{
		_hp.Insert(data);
	}

	void Pop()
	{
		_hp.Remove();
	}

	const T& Top()const
	{
		_hp[0];
	}

	size_t Size()const
	{
		_hp.Size();
	}

	bool Empty()const
	{
		_hp.Empty();
	}

protected:
	Heap<T, Compare> _hp;
};


int main()
{
	int array[] = { 53, 117, 78, 9, 45, 65, 87, 23 };
	//PriorityQueue<int> Pq;
	//Pq.Push(1);
	Heap<int,Greater<int>> hp(array,sizeof(array)/sizeof(array[0]));//创建大堆
	
	//hp.Insert(1);
	//hp.Remove();
	return 0;
}