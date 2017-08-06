#include <iostream>
using namespace std;


enum COLOR{ RED, BLACK };


template<class K, class V>
struct RBTreeNode
{
	RBTreeNode(const K& key = K(), const V& value = V(), const COLOR& color = RED)
	:_pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _key(key)
	, _value(value)
	, _color(color)
	{}

	RBTreeNode<K, V>* _pLeft;
	RBTreeNode<K, V>* _pRight;
	RBTreeNode<K, V>* _pParent;
	K _key;
	V _value;
	COLOR _color;
};


template<class K, class V, class Ref, class Pointer>
class RBTreeIterator
{
	typedef RBTreeNode<K, V> Node;
	typedef RBTreeIterator<K, V, Ref, Pointer> Self;
public:
	RBTreeIterator()
		: _pNode(NULL)
	{}

	RBTreeIterator(Node* pNode)
		: _pNode(pNode)
	{}

	RBTreeIterator(RBTreeIterator& it)
		: _pNode(it._pNode)
	{}

	Self& operator++()
	{
		_Increment();
		return *this;

	}
	Self operator++(int)
	{
		Self pCur(*this);
		_Increment();
		return pCur;
	}
	Self& operator--()
	{
		_Decrement();
		return *this;
	}
	Self operator--(int)
	{
		Self pCur(*this);
		_Decrement();
		return pCur;
	}
	Ref operator*()
	{
		return _pNode->_key;
	}
	const Ref operator*()const
	{
		return _pNode->_key;
	}
	Pointer operator->()
	{
		return &(_pNode->_key);
	}
	const Pointer operator->()const
	{
		return &(_pNode->_key);
	}
	bool operator==(const Self& it)
	{
		return _pNode->_key == it._pNode->_key;
	}
	bool operator!=(const Self& it)
	{
		return _pNode->_key != it._pNode->_key;
	}
protected:
	void _Increment()
	{
		if (_pNode->_pRight)
		{
			_pNode = _pNode->_pRight;
			while (_pNode->_pLeft)
				_pNode = _pNode->_pLeft;
		}
		else
		{
			Node* pParent = _pNode->_pParent;
			while (pParent->_pRight == _pNode)
			{
				_pNode = pParent;
				pParent = pParent->_pParent;
			}
			if (_pNode->_pRight != pParent)
				_pNode = pParent;
		}
	}
	void _Decrement()
	{
		if (_pNode->_color == RED && _pNode->_pParent->_pParent == _pNode)
		{
			_pNode = _pNode->_pRight;
		}
		else if (_pNode->_pLeft)
		{
			_pNode = _pNode->_pLeft;
			while (_pNode->_pRight)
				_pNode = _pNode->_pRight;
		}
		else
		{
			Node* parent = _pNode->_pParent;
			while (parent->_pLeft == _pNode)
			{
				_pNode = parent;
				parent = parent->_pParent;
			}
			_pNode = parent;
		}
	}
protected:
	Node* _pNode;
};



template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;
public:
	typedef RBTreeIterator<K, V, K&, K*> Iterator;
public:
	RBTree()
	{
		_pHead = new Node();
		_pHead->_pLeft = _pHead;
		_pHead->_pRight = _pHead;
		_pHead->_pParent = NULL;
	}
	Iterator Begin()
	{
		return _pHead->_pLeft;
	}
	Iterator End()
	{
		return _pHead;
	}
	bool Empty()const
	{
		return _pHead->_pParent == NULL;
	}
	size_t Size()const
	{
		return _size;
	}


	// Ê×ÏÈ£ºËÑË÷Ê÷
	bool Insert(const K& key, const V& value)
	{
		Node* pRoot = _GetRoot();
		if (pRoot == NULL)
		{
			pRoot = new Node(key, value, BLACK);
			pRoot->_pParent = _pHead;
			_pHead->_pLeft = pRoot;
			_pHead->_pRight = pRoot;
			_pHead->_pParent = pRoot;
			++_size;
			return true;
		}
		Node* pCur = pRoot;
		Node* parent = NULL;
		while (pCur)
		{
			if (key < pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (key > pCur->_key)
			{
				parent = pCur;
				pCur = pCur->_pRight;
			}
			else
				return false;
		}
		pCur = new Node(key, value);
		if (key < parent->_key)
			parent->_pLeft = pCur;
		else
			parent->_pRight = pCur;
		pCur->_pParent = parent;
		while (pCur != pRoot && parent->_color == RED)
		{
			Node* grandf = parent->_pParent;
			if (grandf->_pLeft == parent)
			{
				Node* uncle = grandf->_pRight;
				if (uncle && uncle->_color == RED)
				{
					uncle->_color = BLACK;
					parent->_color = BLACK;
					grandf->_color = RED;
					pCur = grandf;
					parent = pCur->_pParent;
				}
				else
				{
					if (pCur == parent->_pRight)
					{
						_RotateL(parent);
						std::swap(parent, pCur);
					}
					parent->_color = BLACK;
					grandf->_color = RED;
					_RotateR(grandf);
				}
			}
			else
			{
				Node* uncle = grandf->_pLeft;
				if (uncle && uncle->_color == RED)
				{
					uncle->_color = BLACK;
					parent->_color = BLACK;
					grandf->_color = RED;
					pCur = grandf;
					parent = pCur->_pParent;
				}
				else
				{
					if (pCur == parent->_pLeft)
					{
						_RotateR(parent);
						std::swap(parent, pCur);
					}
					parent->_color = BLACK;
					grandf->_color = RED;
					_RotateL(grandf);
				}
			}
		}
		_pHead->_pLeft = _GetMinNode();
		_pHead->_pRight = _GetMaxNode();
		_pHead->_pParent->_color = BLACK;
		++_size;
		return true;
	}
	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_GetRoot());
		cout << endl;
	}

	bool CheckRBTree()
	{
		Node* pRoot = _GetRoot();
		if (pRoot == NULL)
			return true;
		if (pRoot->_color == RED)
			return false;
		int count = 0;
		Node* pCur = pRoot;
		while (pCur)
		{
			if (pCur->_color == BLACK)
				count++;
			pCur = pCur->_pLeft;
		}
		int cout = 0;
		return _CheckRBTree(pRoot, count, cout);
	}

	Iterator Find(const K& key)
	{
		Node* pCur = _GetRoot;
		while (pCur)
		{
			if (key < pCur->_key)
				pCur = pCur->_pLeft;
			else if (key > pCur->_key)
				pCur = pCur->_pRight;
			return pCur;
		}
		return NULL;
	}

private:
	bool _CheckRBTree(Node* pRoot, const size_t blackCount, size_t k)
	{
		if (pRoot == NULL)
			return k == blackCount;

		if (pRoot->_color == RED && RED == pRoot->_pParent->_color)
			return false;
		if (pRoot->_color == BLACK)
			k++;
		return _CheckRBTree(pRoot->_pLeft, blackCount, k) && _CheckRBTree(pRoot->_pRight, blackCount, k);

	}
	void _RotateL(Node* parent)
	{
		Node* pSubR = parent->_pRight;
		Node* pSubRL = pSubR->_pLeft;
		parent->_pRight = pSubRL;
		if (pSubRL)
			pSubRL->_pParent = parent;
		Node* pParent = parent->_pParent;
		pSubR->_pParent = pParent;
		pSubR->_pLeft = parent;
		parent->_pParent = pSubR;
		if (pParent != _pHead)
		{
			if (pParent->_pLeft == parent)
				pParent->_pLeft = pSubR;
			else
				pParent->_pRight = pSubR;
		}
		else
		{
			pParent->_pParent = pSubR;
		}
	}
	void _RotateR(Node* parent)
	{
		Node* pSubL = parent->_pLeft;
		Node* pSubLR = pSubL->_pRight;
		parent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = parent;
		Node* pParent = parent->_pParent;
		pSubL->_pParent = pParent;
		pSubL->_pRight = parent;
		parent->_pParent = pSubL;
		if (pParent != _pHead)
		{
			if (pParent->_pLeft == parent)
				pParent->_pLeft = pSubL;
			else
				pParent->_pRight = pSubL;
		}
		else
		{
			pParent->_pParent = pSubL;
		}
	}
	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}

	Node* &_GetRoot()
	{
		return _pHead->_pParent;
	}

	Node* _GetMinNode()
	{
		Node* pCur = _GetRoot();
		if (pCur)
		{
			while (pCur->_pLeft)
				pCur = pCur->_pLeft;
			return pCur;
		}
		return _pHead;
	}
	Node* _GetMaxNode()
	{
		Node* pCur = _GetRoot();
		if (pCur)
		{
			while (pCur->_pRight)
				pCur = pCur->_pRight;
			return pCur;
		}
		return _pHead;
	}


private:
	Node* _pHead;
	size_t _size;
};


void TestRBTree()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> t;
	for (int idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		t.Insert(a[idx], idx);

	t.InOrder();
	t.Size();
	if (t.CheckRBTree())
	{
		cout << "ÊÇºìºÚÊ÷" << endl;
	}
	else
	{
		cout << "²»ÊÇºìºÚÊ÷" << endl;
	}
}

void TestRBTreeIterator()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> t;
	for (int idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		t.Insert(a[idx], idx);
	RBTree<int, int>::Iterator it = t.Begin();
	while (it != t.End())
	{	
		cout << *it << " ";
		++it;
	}
	cout << endl;
	cout<<t.Empty()<<endl;
	cout << t.Size() << endl;

	//RBTree<int, int>::Iterator it = t.End();
	//while (it != t.Begin())
	//{
	//	--it;
	//	cout << *it << " ";
	//}
	//cout << endl;
}

int main()
{
	//TestRBTree();
	TestRBTreeIterator();
	return 0;
}