#include "iostream"
using namespace std;

template<class K, class V>
struct AVLTreeNode
{
	AVLTreeNode(const K& key, const V& value)
	: _pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	, _key(key)
	, _value(value)
	, _bf(0)
	{}


	AVLTreeNode<K, V> *_pLeft;
	AVLTreeNode<K, V> *_pRight;
	AVLTreeNode<K, V> *_pParent;
	K _key;
	V _value;
	int _bf;       // 平衡因子：right-left
};


template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	AVLTree()
		: _pRoot(NULL)
	{}

	bool Insert(const K& key, const V& value)
	{
		if (_pRoot == NULL)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		Node* pCur = _pRoot;
		Node* pParent = NULL;
		while (pCur)
		{
			if (key < pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->_pLeft;
			}
			else if (key > pCur->_key)
			{
				pParent = pCur;
				pCur = pCur->_pRight;
			}
			else
				return false;
		}
		pCur = new Node(key, value);
		if (key < pParent->_key)
			pParent->_pLeft = pCur;
		else
			pParent->_pRight = pCur;

		pCur->_pParent = pParent;

		if (pParent->_pLeft == pCur)
			pParent->_bf--;
		else
			pParent->_bf++;

		while (pParent)
		{
			if (0 == pParent->_bf)
				return true;
			else if (pParent->_bf == 1 || pParent->_bf == -1)
			{
				pCur = pParent;
				pParent = pParent->_pParent;
				if (pParent)
				{
					if (pParent->_pLeft == pCur)
						pParent->_bf--;
					else
						pParent->_bf++;
				}				
			}
			else
			{
				if (pParent->_bf == -2)
				{
					if (pCur->_bf == -1)
					{
						_RotateR(pParent);
					}
					else
						_RotateLR(pParent);
				}
				else if (pParent->_bf == 2)
				{
					if (pCur->_bf == 1)
					{
						_RotateL(pParent);
					}
					else
						_RotateRL(pParent);
				}
				break;
			}	
		}
		return true;
	}

	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	size_t Height()
	{
		return _Height(_pRoot);
	}

	bool IsBalanceTree()
	{
		return _IsBalanceTree(_pRoot);
	}
private:
	// 检测二叉树是否为平衡树？
	bool _IsBalanceTree(Node* pRoot)
	{
		if (pRoot == NULL)
			return true;
		int LeftHight = _Height(pRoot->_pLeft);
		int RightHight = _Height(pRoot->_pRight);

		if (pRoot->_bf != RightHight - LeftHight || abs(pRoot->_bf) > 1)
			return false;
		return _IsBalanceTree(pRoot->_pLeft) && _IsBalanceTree(pRoot->_pRight);
	}
	size_t _Height(Node* pRoot)
	{
		if (pRoot == NULL)
			return NULL;
		if (pRoot->_pLeft == NULL && pRoot->_pRight == NULL)
			return 1;
		size_t LeftHight = _Height(pRoot->_pLeft);
		size_t RightHight = _Height(pRoot->_pRight);

		return LeftHight > RightHight ? LeftHight + 1 : RightHight + 1;
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

	void _RotateL(Node*& pParent)
	{
		Node* pSubR = pParent->_pRight;
		Node* pSubRL = pSubR->_pLeft;
		pParent->_pRight = pSubRL;
		if (pSubRL)
			pSubRL->_pParent = pParent;
		Node* ppParent = pParent->_pParent;
		pSubR->_pParent = ppParent;
		pSubR->_pLeft = pParent;
		pParent->_pParent = pSubR;
		if (ppParent)
		{
			if (ppParent->_pLeft == pParent)
				ppParent->_pLeft = pSubR;
			else
				ppParent->_pRight = pSubR;
		}
		else
			_pRoot = pSubR;
		pSubR->_bf = pParent->_bf = 0;
	}
	void _RotateR(Node*& pParent)
	{
		Node* pSubL = pParent->_pLeft;
		Node* pSubLR = pSubL->_pRight;
		pParent->_pLeft = pSubLR;
		if (pSubLR)
			pSubLR->_pParent = pParent;
		Node* ppParent = pParent->_pParent;
		pParent->_pParent = pSubL;
		pSubL->_pRight = pParent;
		pSubL->_pParent = ppParent;
		if (ppParent)
		{
			if (ppParent->_pLeft == pParent)
				ppParent->_pLeft = pSubL;
			else
				ppParent->_pRight = pSubL;
		}
		else
			_pRoot = pSubL;
		pSubL->_bf = pParent->_bf = 0;
	}
	void _RotateLR(Node*& pParent)
	{
		Node* ppParent = pParent->_pParent;
		Node* pSubL = pParent->_pLeft;
		Node* pSubLR = pSubL->_pRight;
		int bf = pSubLR->_bf;
		_RotateL(pSubL);
		_RotateR(pParent);
		if (bf == 1)
			pSubL->_bf = -1;
		else if (bf == -1)
			pParent->_bf = 1;

		pSubLR->_pParent = ppParent;
	}

	void _RotateRL(Node*& pParent)
	{
		Node* ppParent = pParent->_pParent;
		Node* pSubR = pParent->_pRight;
		Node* pSubRL = pSubR->_pLeft;
		int bf = pSubRL->_bf;
		_RotateR(pSubR);
		_RotateL(pParent);
		if (bf == 1)
			pParent->_bf = -1;
		else if (bf == -1)
			pSubR->_bf = 1;

		pSubRL->_pParent = ppParent;
	}
private:
	Node* _pRoot;
};


void TestAVL()
{
	int array[] = { 3, 2, 1, 4, 5, 6, 7, 10, 9, 8 };
	//int array[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int, int> t;
	for (size_t idx = 0; idx < sizeof(array) / sizeof(array[0]); ++idx)
		t.Insert(array[idx], idx);
	

	t.InOrder();

	if (t.IsBalanceTree())
	{
		cout << "是AVL树" << endl;
	}
	else
	{
		cout << "不是AVL树" << endl;
	}
}


int main()
{
	TestAVL();
	return 0;
}