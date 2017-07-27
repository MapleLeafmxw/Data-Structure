#include "iostream"
using namespace std;

template<class K, class V>
struct BSTreeNode
{
	typedef BSTreeNode<K, V> Node;
public:
	BSTreeNode(const K key, const V value)
		:_key(key)
		, _value(value)
		, _Left(NULL)
		, _Right(NULL)
		//, _parent(NULL)
	{}
		
	Node* _Left;
	Node* _Right;
	//Node* _parent;
	K _key;
	V _value;
};

template<class K, class V>
class BSTree
{
	typedef BSTreeNode<K, V> Node;
public:
	BSTree()
		:_pRoot(NULL)
	{}

	//BSTree(const T array[], const T& index)
	//{
	//	_pRoot = NULL;
	//	for (size_t i = 0; i < index; ++i)
	//	{
	//		Insert(array[i], i);
	//	}
	//}

	BSTree(const BSTree& bst)
	{
		_pRoot = _CopyBSTree(bst._pRoot);
	}

	BSTree<K, V>& operator=(const BSTree<K, V>& bst)
	{
		if (this != &bst)
		{
			Destroy(_pRoot);
			_pRoot = _CopyBSTree(bst._pRoot);
		}
		return *this;
	}


	~BSTree()
	{
		Destroy(_pRoot);
	}

	void InOrder()
	{
		cout << "InOrder: ";
		_InOrder(_pRoot);
		cout << endl;
	}

	const K& GetMaxKey()const
	{
		Node* pCur = _pRoot;
		if (pCur)
		{
			while (pCur->_Right)
				pCur = pCur->_Right;
			return pCur->_key;
		}
		return -1;
	}
	const K& GetMinKey()const
	{
		Node* pCur = _pRoot;
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
		if (_pRoot == NULL)
		{
			_pRoot = new Node(key, value);
			return true;
		}
		
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
		}
		if (key > pParent->_key)
		{
			Node* Temp = new Node(key, value);
			pParent->_Right = Temp;
		}
		return true;
	}

	bool InsertRes(const K& key, const V& value)
	{
		return _Insert(_pRoot, key, value);
	}

	bool Remove(const K& key)
	{
		if (_pRoot == NULL)
			return false;
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
				return true;
			}
		}
		return false;
	}

	bool RemoveRes(const K& key)
	{
		return _Remove(_pRoot, key);
	}

	Node* Find(const K& key)
	{
		Node* pCur = _pRoot;
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

	Node* FindRes(const K& key)
	{
		return _Find(_pRoot, key);
	}

protected:
	Node* _Find(Node* pRoot, const K& key)
	{
		if (pRoot)
		{
			if (pRoot->_key == key)
				return pRoot;
			else if (key < pRoot->_key)
				return _Find(pRoot->_Left, key);
			else
			return _Find(pRoot->_Right, key);
		}
	}
	bool _Insert(Node* & pRoot, const K& key, const V& value)
	{
		if (pRoot == NULL)
		{
			pRoot = new Node(key, value);
			return true;
		}
		if (key < pRoot->_key)
			return _Insert(pRoot->_Left, key, value);
		if (key > pRoot->_key)
			return _Insert(pRoot->_Right, key, value);
		if (pRoot->_Left == NULL && pRoot->_Right == NULL)
		{
			Node* pCur = new Node(key, value);
			if (key < pRoot->_key)
			{
				pRoot->_Left = pCur;
				return true;
			}			
			else if (key > pRoot->_key)
			{
				pRoot->_Right = pCur;
				return true;
			}
			else
				return false;
		}
		return false;
	}
	bool _Remove(Node*& pRoot, const K& key)
	{
		if (pRoot)
		{
			if (key < pRoot->_key)
				return _Remove(pRoot->_Left, key);
			else if (key > pRoot->_key)
				return _Remove(pRoot->_Right, key);
			else
			{
				Node* Del = pRoot;
				if (pRoot->_Right == NULL)
					pRoot = pRoot->_Left;
				else if (pRoot->_Left == NULL)
					pRoot = pRoot->_Right;
				else
				{
					Node* parent = pRoot;
					Node* pCur = pRoot->_Right;
					while (pCur->_Left)
					{
						parent = pCur;
						pCur = pCur->_Left;
					}
					Del->_key = pRoot->_key;
					Del->_value = pRoot->_value;

					if (parent->_Left = pCur)
						parent->_Left = pCur->_Right;
					else
						parent->_Right = pCur->_Right;
				}
				delete Del;
				Del = NULL;
				return true;
			}
		}
		return false;
	}


private:

	Node* _CopyBSTree(Node* pRoot)
	{
		Node* pCur = NULL;
		if (pRoot)
		{
			Node* lchild = _CopyBSTree(pRoot->_Left);
			Node* rchild = _CopyBSTree(pRoot->_Right);
			pCur = new Node(pRoot->_key, pRoot->_value);
			pCur->_Left = lchild;
			pCur->_Right = rchild;
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
			_InOrder(pRoot->_pLeft);
			cout << pRoot->_key << " ";
			_InOrder(pRoot->_pRight);
		}
	}


private:
	Node* _pRoot;
};

void test1()
{
	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };//删除8没有左孩子有右孩子
	//int a[] = { 5, 7, 8, 6, 9 };//删除5为根节点且没有左子树有右孩子
	//double a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 6.5, 0, 9 };//删除6而且6没有左孩子有右孩子
	//int a[] = { 5, 3, 4, 1, 7, 8, 6, 0, 9 };//删除1且它没有右孩子有左孩子
	//int a[] = { 5, 3, 4, 1, 2, 0,  };//删除根节点切根节点没有左孩子
	//int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	//double a[] = { 5, 3, 4, 1, 4.5, 7, 8, 2, 6, 0, 9 };//删除4且它有左孩子没有右孩子
	BSTree<int, int> bst;
	//BSTree<double, int> bst;
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		bst.Insert(a[i], i);
	}
	//BSTreeNode<int, int>* rec = bst.Find(8);
	//bst.Remove(8);
	//bst.Remove(5);
	//bst.Remove(6);
	//bst.Remove(1);
	//bst.Remove(4);
	//cout<<bst.GetMinKey()<<endl;
	//cout<<bst.GetMaxKey()<<endl;
	BSTree<int, int> bst1(bst);
	//BSTree<int, int> bst1 = bst;
	
}


void test2()
{

	int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };//删除8没有左孩子有右孩子
	//int a[] = { 5, 7, 8, 6, 9 };//删除5为根节点且没有左子树有右孩子
	//double a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 6.5, 0, 9 };//删除6而且6没有左孩子有右孩子
	//int a[] = { 5, 3, 4, 1, 7, 8, 6, 0, 9 };//删除1且它没有右孩子有左孩子
	//int a[] = { 5, 3, 4, 1, 2, 0,  };//删除根节点切根节点没有左孩子
	//int a[] = { 5, 3, 4, 1, 7, 8, 2, 6, 0, 9 };
	//double a[] = { 5, 3, 4, 1, 4.5, 7, 8, 2, 6, 0, 9 };//删除4且它有左孩子没有右孩子
	BSTree<int, int> bst;
	//BSTree<double, int> bst;
	for (int i = 0; i < sizeof(a) / sizeof(a[0]); ++i)
	{
		bst.InsertRes(a[i], i);
	}
	//BSTreeNode<int, int>* rec = bst.Find(8);
	//bst.Remove(8);
	//bst.Remove(5);
	//bst.Remove(6);
	//bst.Remove(1);
	bst.Remove(4);
}

int main()
{
	test1();
	//test2();
	return 0;
}