#include <iostream>
using namespace std;
#include <vector>
#include <string>

enum State
{
	EMPTY, EXIST, DELETE
};

template<class K, class V>
struct HashElem
{
	pair<K, V>  _kv;
	State _s;

	HashElem()
		: _s(EMPTY)
	{}
};

template<class K>
struct _HashFun
{
	size_t operator()(const K& key)
	{
		return key;
	}
};
template<>
struct _HashFun <string>
{

	size_t operator()(const string& str)
	{
		return BKDRHash(str.c_str());
	}	
	static size_t BKDRHash(const char * str)
	{
		unsigned int seed = 131; // 31 131 1313 13131 131313	
		unsigned int hash = 0;
		while (*str)
		{
			hash = hash * seed + (*str++);
		}
		return (hash & 0x7FFFFFFF);
	}
	
};


size_t GetNextPrime(size_t size)
{
	const int _PrimeSize = 28;
	static const unsigned long _PrimeList[_PrimeSize] =
	{
		53ul, 97ul, 193ul, 389ul, 769ul,
		1543ul, 3079ul, 6151ul, 12289ul, 24593ul,
		49157ul, 98317ul, 196613ul, 393241ul, 786433ul,
		1572869ul, 3145739ul, 6291469ul, 12582917ul, 25165843ul,
		50331653ul, 100663319ul, 201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};
	for (size_t i = 0; i < _PrimeSize; i++)
	{
		if (size < _PrimeList[i])
			return _PrimeList[i];
	}
	return 0;
}


// 线性探测
template<class K, class V, typename HashFunc = _HashFun<K>> /*bool IsLine = true*/
class HashTable
{
public:
	HashTable(size_t size = 12)
		: _size(0)
	{
		_table.resize(GetNextPrime(size));
	}

	bool Insert(const K& key, const V& value)
	{
		_CheckTable();
		size_t idx = _HashFunc(key);
		while (_table[idx]._s == EXIST)
		{
			if (_table[idx]._kv.first == key)
				return false;
			idx++;
			if (idx == _table.size())
				idx = 0;
		}
		_table[idx]._kv.first = key;
		_table[idx]._kv.second = value;
		_table[idx]._s = EXIST;
		++_size;
		return true;
	}

	pair<HashElem<K, V>*, bool> Find(const K& key)
	{
		size_t idx = _HashFunc(key);
		while (_table[idx]._s != EMPTY)
		{
			if (_table[idx]._kv.first == key)
				return make_pair(&_table[idx], true);
			idx++;
			if (idx == _table.size())
				idx = 0;
		}
		return make_pair((HashElem<K, V>*)0, false);
	}

	bool Remove(const K& key)
	{
		pair<HashElem<K, V>*, bool> rec = Find(key);
		if (rec.second == true)
		{
			rec.first->_s = DELETE;
			--_size;
			return true;
		}
		else
			return false;

	}

private:
	size_t _HashFunc(const K& key)
	{
		return _HashFun<K>()(key) % _table.size();
	}
	// 线性探测处理函数
	//size_t HashFunc1(size_t hashAddr);
	//// 二次探测处理函数
	//size_t HashFunc2(size_t hashAddr, size_t i);

	void _CheckTable()
	{
		if (_size * 10 / _table.size() > 7)
		{
			HashTable<int, int> hash;
			hash._table.resize(_table.size() * 2);
			size_t idx = 0;
			while (idx < _table.size())
			{
				if (_table[idx]._s == EXIST)
					hash.Insert(_table[idx]._kv.first, _table[idx]._kv.second);
				idx++;
			}
			Swap(hash);
		}
	}

	void Swap(HashTable<K, V>& ht)
	{
		_table.swap(ht._table);
		swap(_size, ht._size);
	}

private:
	std::vector<HashElem<K, V>> _table;
	size_t _size;  // 有效元素的个数
};

void Hashtest1()
{
	int array[] = { 37, 25, 14, 36, 49, 68, 57, 11, };
	HashTable<int, int> ht;
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		ht.Insert(array[i], i);
	}
	//ht.Remove(37);
	int a = 0;
}


int main()
{
	Hashtest1();
	return 0;
}