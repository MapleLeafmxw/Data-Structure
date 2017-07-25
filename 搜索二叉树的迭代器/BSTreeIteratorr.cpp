#include "iostream"
using namespace std;


template<class K, class V>
struct BSTreeNode
{
	typedef BSTreeNode<K, V> Node;
public:
	BSTreeNode(const K& key = K(), const V& value = V())
		:_key(key)
		, _value(value)
		, _Left(NULL)
		, _Right(NULL)
		, _parent(NULL)
	{}

	Node* _Left;
	Node* _Right;
	Node* _parent;
	K _key;
	V _value;
};



template<class K, class V, class Ref, class Ptr>
class BSTIterator
{
	typedef BSTreeNode<K, V> Node;
	typedef BSTIterator<K, V, Ref, Ptr> Self;
public:
	BSTIterator()
		: _pNode(NULL)
	{}

	BSTIterator(Node* pNode)
		: _pNode(pNode)
	{}

	BSTIterator(const Self& s)
	{
		_pNode = s._pNode;
	}

	Self& operator++()
	{
		Increment(_pNode);
		return *this;
	}
	Self operator++(int)
	{
		Self s(*this);
		Increment(_pNode);
		return s;
	}
	Self& operator--()
	{
		Decrement(_pRoot);
		return *this;
	}
	Self operator--(int)
	{
		Self s(*this);
		Decrement(_pRoot);
		return s;
	}

	Ref operator*()
	{
		return _pNode->_key;
	}
	Ptr operator->()
	{
		return _pNode;
	}
	bool operator!=(const Self& s)
	{
		return _pNode != s._pNode;
	}
	bool operator==(const Self& s)
	{
		return !(_pNode != s._pNode);
	}
	


protected:


	// 取当前结点的下一个结点
	void Increment(Node*& pNode)
	{
		if (pNode->_Right)
		{
			pNode = pNode->_Right;
			while (pNode->_Left)
				pNode = pNode->_Left;
		}
		else
		{
			Node* pParent = pNode->_parent;
			while (pParent->_Right == pNode)
			{
				pNode = pParent;
				pParent = pParent->_parent;
			}
			if (pNode->_Right != pParent)
				pNode = pParent;
		}
	}

	// --取前一个小的结点，在left子树中
	void Decrement(Node*& pRoot)
	{
		if (pRoot->_Left)
		{
			pRoot = pRoot->_Left;
			while (pNode->_Right)
				pNode = pNode->_Right;

		}
		else
		{
			Node* pParent = pNode->_parent;
			while (pParent->_Left == pNode)
			{
				pNode = pParent;
				pParent = pParent->_parent;
			}
				pNode = pParent;
		}
	}

protected:
	Node* _pNode;
};


template<class K, class V>
class BSTree
{
	typedef BSTreeNode<K, V> Node;
public:
	typedef BSTIterator<K, V, K&, Node*> Iterator;
	BSTree()
	{
		_pHead = new Node();
		_pHead->_Left = _pHead;
		_pHead->_Right = _pHead;
		_pHead->_parent = NULL;
	}


	BSTree(const BSTree& bst)
	{
		_pHead = new Node();
		_pHead->_Left = _pHead;
		_pHead->_Right = _pHead;
		_pHead->_parent = NULL;
		Node* _pRoot = bst._pHead->_parent;
		_pHead->_parent = _CopyBSTree(_pRoot);
		_pHead->_parent->_parent = _pHead;
	}

	BSTree<K, V>& operator=(const BSTree<K, V>& bst)
	{
		Node* _pRoot = bst._pHead->_parent;
		Node* pRoot = _pHead->_parent;
		if (this != &bst)
		{
			Destroy(pRoot);
			_pHead->_parent = _CopyBSTree(_pRoot);
			_pHead->_parent->_parent = _pHead;
		}
		return *this;
	}




	~BSTree()
	{
		Destroy(_pHead->_parent);
	}

	void InOrder()
	{
		Node* _pRoot = _GetpRoot();
		cout << "InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	K GetMaxKey()
	{
		Node* pCur = _GetpRoot();
		if (pCur)
		{
			while (pCur->_Right)
				pCur = pCur->_Right;
			return pCur->_key;
		}
		return -1;
	}
	K GetMinKey()
	{
		Node* pCur = _GetpRoot();
		if (pCur)
		{
			while (pCur->_Left)
				pCur = pCur->_Left;
			return pCur->_key;
		}
		return -1;
	}



	bool Insert(const K& key, const V& value)
	{
		Node* _pRoot = _GetpRoot();
		if (_pRoot == NULL)
		{
			_pRoot = new Node(key, value);
			_pRoot->_parent = _pHead;
			_pHead->_Left = _pRoot;
			_pHead->_Right = _pRoot;
			_pHead->_parent = _pRoot;
			return true;
		}
		else
		{
			Node* pCur = _pRoot;
			Node* pParent = NULL;
			while (pCur)
			{
				if (key == pCur->_key)
					return false;
				else if (key < pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_Left;
				}
				else
				{
					pParent = pCur;
					pCur = pCur->_Right;
				}
			}
			if (key < pParent->_key)
			{
				Node* Temp = new Node(key, value);
				pParent->_Left = Temp;
				Temp->_parent = pParent;
			}
			if (key > pParent->_key)
			{
				Node* Temp = new Node(key, value);
				pParent->_Right = Temp;
				Temp->_parent = pParent;
			}
			_pHead->_Left = _LeftMost();
			_pHead->_Right = _RightMost();
			return true;
		}
		return false;
		
	}


	bool Remove(const K& key)
	{
		Node*_pRoot = _GetpRoot();
		if (_pRoot == NULL)
			return false;
		if (NULL == _pRoot->_Left && NULL == _pRoot->_Right && _pRoot->_key == key)
		{
			delete _pRoot;
			_pRoot = NULL;
			return true;
		}
		else
		{
			Node* pCur = _pRoot;
			Node* pParent = NULL;
			while (pCur)
			{
				if (key < pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_Left;
				}
				else if (key > pCur->_key)
				{
					pParent = pCur;
					pCur = pCur->_Right;
				}
				else //找到了	
				{
					//没有右子树，只有左
					if (pCur->_Right == NULL)
					{
						if (pCur == _pRoot)//删除的为根节点
							_pRoot = pCur->_Left;
						else
						{
							if (pParent->_Right == pCur)//删除的为pParent的左孩子
								pParent->_Right = pCur->_Left;
							else
								pParent->_Left = pCur->_Left;//删除的为pParent的右孩子
						}
					}
					//没有左子树，只有右
					else if (pCur->_Left == NULL)
					{
						if (pCur == _pRoot)//删除的为根节点
							_pRoot = pCur->_Right;
						else
						{
							if (pParent->_Left == pCur)//删除的为pParent的左孩子
								pParent->_Left = pCur->_Right;
							else
								pParent->_Right = pCur->_Right;//删除的为pParent的右孩子
						}
					}
					else
					{
						Node* Temp = _pRoot;
						pParent = pCur;
						pCur = pCur->_Right;
						while (pCur->_Left)
						{
							pParent = pCur;
							pCur = pCur->_Left;
						}
						Temp->_key = pCur->_key;
						Temp->_value = pCur->_value;
						if (Temp->_Right == pCur)//删除的为根节点的右子树
							pParent->_Right = pCur->_Right;
						else  //删除的是根节点左子树中最小的也是最左边的值
							pParent->_Left = pCur->_Right;
					}
					delete pCur;
					pCur = NULL;
				}
			}
			_pHead->_Left = _LeftMost();
			_pHead->_Right = _RightMost();
			return true;
		}

	}


	Node* Find(const K& key)
	{
		Node* pCur = _GetpRoot();
		while (pCur)
		{
			if (pCur->_key == key)
				return pCur;
			else if (key < pCur->_key)
			{
				pCur = pCur->_Left;
			}
			else
			{
				pCur = pCur->_Right;
			}
		}
		return NULL;
	}

	Node* Begin()
	{
		return _pHead->_Left;
	}
	Node* End()
	{
		return _pHead;
	}

	Node* List()
	{
		Node* pCur = _GetpRoot();
		Node* pRoot = pCur;
		if (pCur == NULL)
			return NULL;
		while (pCur->_Left)
			pCur = pCur->_Left;

		Node* prev = NULL;
		_List(pRoot, prev);

		return pCur;
	}

private:

	void _List(Node* pCur, Node*& prev)
	{
		if (pCur)
		{
			_List(pCur->_Left, prev);
			pCur->_Left = prev;
			if (prev)
				prev->_Right = pCur;
			prev = pCur;
			_List(pCur->_Right, prev);
		}
	}

	Node* _GetpRoot()
	{
		return _pHead->_parent;
	}

	Node* _LeftMost()
	{
		Node* pCur = _GetpRoot();
		if (pCur)
		{
			while (pCur->_Left)
				pCur = pCur->_Left;
			return pCur;
		}
		return _pHead;
	}

	Node* _RightMost()
	{
		Node* pCur = _GetpRoot();
		if (pCur)
		{
			while (pCur->_Right)
				pCur = pCur->_Right;
			return pCur;
		}
		return _pHead;
	}

	Node* _CopyBSTree(Node*& pRoot)
	{
		Node* pCur = NULL;
		if (pRoot)
		{
			Node* lchild = _CopyBSTree(pRoot->_Left);
			Node* rchild = _CopyBSTree(pRoot->_Right);
			pCur = new Node(pRoot->_key, pRoot->_value);
			pCur->_Left = lchild;
			pCur->_Right = rchild;
			if (lchild)
				lchild->_parent = pCur;
			if (rchild)
				rchild->_parent = pCur;
		}
		return pCur;
	}
	void Destroy(Node* pRoot)
	{
		if (pRoot)
		{
			Destroy(pRoot->_Left);
			Destroy(pRoot->_Right);

			delete pRoot;
			pRoot = NULL;
		}
	}


	void _InOrder(Node* pRoot)
	{
		if (pRoot)
		{
			_InOrder(pRoot->_Left);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_Right);
		}
	}

protected:
	Node* _pHead;

	
};

int main()
{
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	BSTree<int, int> bst;
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		bst.Insert(a[i], i);
	}

	BSTree<int, int>::Iterator it = bst.Begin();
	while (it != bst.End())
	{
		cout << *it << " ";
		++it;
	}
	//BSTree<int, int> bst1(bst);
	it = bst.List();
	////bst1 = bst;
	//int c = bst.GetMaxKey();
	//int b = bst.GetMinKey();
	//bst.InOrder();
	//bst.Remove(5);
	//bst.InOrder();
	return 0;
}