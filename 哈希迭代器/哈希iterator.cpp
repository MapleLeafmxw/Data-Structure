#include <iostream>
using namespace std;
#include <vector>


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


template<class K, class V>
struct HashNode
{
	HashNode(const pair<K, V>& kv)
	: _kv(kv)
	, _pNext(NULL)
	{}

	pair<K, V> _kv;
	HashNode<K, V>* _pNext;
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

template<class K, class V>
class HashTable;

// 迭代器
template<class K, class V, class Ref, class Ptr>
class HashTableIterator
{
	typedef HashNode<K, V> Node;
	typedef HashTableIterator<K, V, Ref, Ptr> Self;
public:
	HashTableIterator()
		: _pNode(NULL)
		, _ht(NULL)
	{}

	HashTableIterator(Node* pNode, HashTable<K, V>* ht)
		: _pNode(pNode)
		, _ht(ht)
	{}

	HashTableIterator(const Self& s)
		: _pNode(s._pNode)
		, _ht(s._ht)
	{}

	Self& operator++()
	{
		Next();
		return *this;
	}
	Self operator++(int)
	{
		Node Temp(*this);
		Next();
		return Temp;
	}

	Ref operator*()
	{
		return _pNode->_kv;
	}
	Ptr operator->()
	{
		return &(_pNode->_kv);
		//return &(operator*());
	}
	bool operator==(const Self& s)
	{
		return _pNode == s._pNode;
	}
	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}
private:
	void Next()
	{
		if (_pNode->_pNext == NULL)
		{
			size_t idx = _ht->_HashFunc(_pNode->_kv.first) + 1;
			while (_ht->_table[idx] == NULL)
			{
				idx++;
				if (idx > _ht->_table.size() - 1)
				{
					_pNode = NULL;
					return;
				}
			}
			_pNode = _ht->_table[idx];
		}
		else
		{
			_pNode = _pNode->_pNext;
		}
	}
private:
	Node* _pNode;
	HashTable<K, V>* _ht;
};

// 作业--->
//哈希桶
template<class K, class V>
class HashTable
{
	typedef HashNode<K, V> Node;
	friend  class HashTableIterator<K, V, pair<K, V>&, pair<K, V>*>;
public:
	typedef HashTableIterator<K, V, pair<K, V>&, pair<K, V>*> Iterator;

public:
	HashTable(size_t size = 13)
		: _size(0)
	{
		_table.resize(GetNextPrime(size));
	}

	~HashTable()
	{
		Clear();
	}

	Iterator Begin()
	{
		size_t idx = 0;
		for (; idx < _table.size(); idx++)
		{
			if (_table[idx] != NULL)
				return  Iterator(_table[idx], this);
		}
	}
	Iterator End()
	{
		return Iterator(NULL, this);
	}
	bool InsertEqual(const K& key, const V& value)
	{
		CheckCapacity();
		size_t idx = _HashFunc(key);
		pair<K, V> temp(key, value);
		Node* pCur = new Node(temp);
		pCur->_pNext = _table[idx];
		_table[idx] = pCur;
		++_size;
		return true;
	}
	bool RemoveEqual(const K& key)
	{
		size_t idx = _HashFunc(key);
		Node* pPre = NULL;
		Node* pCur = _table[idx];
		size_t count = 0;
		while (pCur)
		{
			if (pCur->_kv.first == key)
			{
				if (_table[idx] == pCur)
				{
					_table[idx] = pCur->_pNext;
					delete pCur;
					++count;
					--_size;
					pCur = _table[idx];
				}
				else
				{
					pPre->_pNext = pCur->_pNext;
					delete pCur;
					++count;
					--_size;
					pCur = pPre->_pNext;
				}
			}
			else
			{
				pPre = pCur;
				pCur = pCur->_pNext;
			}
		}
		if (count > 0)
		{
			return true;
		}
		return false;
	}

	pair<Node*, bool> InsertUnique(const K& key, const V& value)
	{
		CheckCapacity();
		size_t idx = _HashFunc(key);
		pair<K, V> temp(key, value);
		Node* pCur = new Node(temp);
		pair<Node*, bool> hs = Find(key);
		if (hs.second == true)
			return make_pair((Node*)0, false);
		pCur->_pNext = _table[idx];
		_table[idx] = pCur;
		++_size;
	}
	bool RemoveUnique(const K& key)
	{
		size_t idx = _HashFunc(key);
		pair<Node*, bool> hs = Find(key);
		if (hs.second == false)
			return false;
		Node* pPre = NULL;
		Node* pCur = _table[idx];
		while (pCur)
		{
			if (pCur->_kv.first == key)
			{
				if (_table[idx] = pCur)
					_table[idx] = pCur->_pNext;
				else
					pPre->_pNext = pCur->_pNext;
				delete pCur;
				--_size;
				return true;
			}
			pPre = pCur;
			pCur = pCur->_pNext;
		}
		return false;
	}

	pair<Node*, bool> Find(const K& key)
	{
		size_t idx = _HashFunc(key);
		Node* pCur = _table[idx];
		while (pCur)
		{
			if (pCur->_kv.first == key)
				return make_pair(pCur, true);
			else
				pCur = pCur->_pNext;
		}
		return make_pair((Node*)0, false);
	}
	void Clear()
	{
		for (size_t i = 0; i < _table.size(); i++)
		{
			Node* pCur = _table[i];
			while (pCur)
			{
				_table[i] = pCur->_pNext;
				delete pCur;
				--_size;
				pCur = _table[i];
			}
		}
	}
	size_t Size()const
	{
		return _size;
	}

	bool Empty()const
	{
		return _size == 0;
	}

private:
	void CheckCapacity()
	{
		if (_size == _table.size())
		{
			HashTable<K, V> hs;
			hs._table.resize(GetNextPrime(_size));
			for (size_t i = 0; i < _size; i++)
			{
				Node* pCur = _table[i];
				while (pCur)
				{
					hs.InsertEqual(pCur->_kv.first, pCur->_kv.second);
					pCur = pCur->_pNext;
				}
			}
			Swap(hs);
		}
	}

	size_t _HashFunc(const K& key)
	{
		return _HashFun<K>()(key) % _table.size();
	}
	void Swap(HashTable<K, V>& ht)
	{
		_table.swap(ht._table);
		swap(_size, ht._size);
	}
private:
	vector<Node*> _table;
	size_t _size;   // 哈希表中有效元素的个数
};

void testUnique()
{
	int array[] = { 37, 25, 14, 36, 49, 68, 57, 11, 25 };
	HashTable<int, int> hash;
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		hash.InsertUnique(array[i], i);
	}
	hash.RemoveUnique(37);
	cout << hash.Size() << endl;
	cout << hash.Empty() << endl;
}

void testEqual()
{
	int array[] = { 37, 25, 14, 36, 49, 68, 57, 11, 25, 53, 106, 53, 106, 212, 53, };
	HashTable<int, int> hash;
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		hash.InsertEqual(array[i], i);
	}
	hash.RemoveEqual(53);
	cout << hash.Size() << endl;
	cout << hash.Empty() << endl;
	hash.Clear();
	cout << hash.Size() << endl;
	cout << hash.Empty() << endl;
}

void test()
{
	int array[] = { 37, 25, 14, 36, 49, 68, 57, 11, 25 };
	HashTable<int, int> hash;
	for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
	{
		hash.InsertUnique(array[i], i);
	}
	hash.RemoveUnique(37);
	cout << hash.Size() << endl;
	cout << hash.Empty() << endl;
	HashTable<int, int>::Iterator it = hash.Begin();
	while (it != hash.End())
	{
		cout << (*it).first << endl;
		++it;
	}
}

int  main()
{
	//testUnique();
	//testEqual();
	test();
	return 0;
}