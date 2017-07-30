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

template<class K, class V>
class RBTree
{
	typedef RBTreeNode<K, V> Node;

public:
	RBTree()
		:_pRoot(NULL)
	{}


	// Ê×ÏÈ£ºËÑË÷Ê÷
	bool Insert(const K& key, const V& value)
	{
		if (_pRoot == NULL)
		{
			_pRoot = new Node(key, value, BLACK);
			return true;
		}
		Node* pCur = _pRoot;
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
		while (pCur != _pRoot && parent->_color == RED)
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
		_pRoot->_color = BLACK;
		return true;
	}
	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	bool CheckRBTree()
	{
		if (_pRoot == NULL)
			return true;
		if (_pRoot->_color == RED)
			return false;
		int count = 0;
		Node* pCur = _pRoot;
		while (pCur)
		{
			if (pCur->_color == BLACK)
				count++;
			pCur = pCur->_pLeft;
		}
		int cout = 0;
		return _CheckRBTree(_pRoot, count, cout);
	}

	Node* Find(const K& key)
	{
		Node* pCur = _pRoot;
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
		if (pParent)
		{
			if (pParent->_pLeft == parent)
				pParent->_pLeft = pSubR;
			else
				pParent->_pRight = pSubR;
		}
		else
		{
			_pRoot = pSubR;
			_pRoot->_pParent = NULL;
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
		if (pParent)
		{
			if (pParent->_pLeft == parent)
				pParent->_pLeft = pSubL;
			else
				pParent->_pRight = pSubL;
		}
		else
		{
			_pRoot = pSubL;
			_pRoot->_pParent = NULL;
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

private:
	Node* _pRoot;
};


void TestRBTree()
{
	int a[] = { 10, 7, 8, 15, 5, 6, 11, 13, 12 };
	RBTree<int, int> t;
	for (int idx = 0; idx < sizeof(a) / sizeof(a[0]); ++idx)
		t.Insert(a[idx], idx);

	t.InOrder(); 

	if (t.CheckRBTree())
	{
		cout << "ÊÇºìºÚÊ÷" << endl;
	}
	else
	{
		cout << "²»ÊÇºìºÚÊ÷" << endl;
	}
}

int main()
{
	TestRBTree();
	return 0;
}