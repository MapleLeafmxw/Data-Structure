#include <iostream>
using namespace std;

template<class K, size_t M>
struct BTreeNode
{
	//K _keys[M-1];     // 关键字的集合
	//BTreeNode<K, M> _pSub[M];    // 孩子的集合

	// 多给一个关键字：为简化分裂的逻辑
	K _keys[M];     // 关键字的集合
	BTreeNode<K, M>* _pSub[M + 1];    // 孩子的集合
	BTreeNode<K, M>* _pParent;
	size_t _size; // 有效关键字的个数

	BTreeNode()
		: _size(0)
		, _pParent(NULL)
	{
		for (size_t idx = 0; idx < M + 1; ++idx)
			_pSub[idx] = NULL;

	}
};


template<class K, size_t M>
class BTree
{
	typedef BTreeNode<K, M> Node;
public:
	BTree()
		: _pRoot(NULL)
	{}

	pair<Node*, int> Find(const K& key)
	{
		Node* pCur = _pRoot;
		Node* parent = NULL;
		while (pCur)
		{
			size_t i = 0;
			while (i < _pRoot->_size)
			{
				if (key < pCur->_keys[i])
					break;
				else if (key > pCur->_keys[i])
					i++;
				else
					return pair<Node*, int>(pCur, i);
			}
			parent = pCur;
			pCur = pCur->_pSub[i];
		}
		return pair<Node*, int>(parent, -1);
	}
	bool Insert(const K& key)
	{
		if (_pRoot == NULL)
		{
			_pRoot = new Node();
			_pRoot->_keys[0] = key;
			_pRoot->_size = 1;
			return true;
		}
		//找插入位置
		pair<Node*, int> product = Find(key);
		if (product.second != -1)
		{
			return false;
		}
		Node* pCur = product.first;
		Node* pSub = NULL;
		K Newkey = key;
		while (1)
		{
			_InsertKey(pCur, Newkey, pSub);
			if (pCur->_size >= M)
			{
				size_t mid = pCur->_size >> 1;
				size_t i = 0;
				size_t j = mid + 1;
				Node* pNewNode = new Node();
				for (; j < pCur->_size; i++, j++)
				{
					pNewNode->_keys[i] = pCur->_keys[j];
					pNewNode->_pSub[i] = pCur->_pSub[j];
					pNewNode->_size++;
					pCur->_size--;
				}
				pNewNode->_pSub[i] = pCur->_pSub[j];
				if (pCur == _pRoot)
				{
					Node* parent = new Node();
					parent->_keys[parent->_size] = pCur->_keys[mid];
					parent->_pSub[parent->_size] = pCur;
					parent->_pSub[parent->_size + 1] = pNewNode;
					parent->_size++;
					pCur->_size--;
					pNewNode->_pParent = parent;
					pCur->_pParent = parent;
					_pRoot = parent;
					return true;
				}
				else
				{
					Newkey = pCur->_keys[mid];
					pSub = pNewNode;
					pCur->_size--;
					pCur = pCur->_pParent;
				}
			}
			else
				return true;
		}
	}
	void InOrder()
	{
		cout << " InOrder:";
		_InOrder(_pRoot);
		cout << endl;
	}

private:
	void _InOrder(Node* pRoot)
	{
		if (pRoot == NULL)
			return;	
		size_t idx = 0;
		for (idx = 0; idx < pRoot->_size; idx++)
		{
			_InOrder(pRoot->_pSub[idx]);
			cout << pRoot->_keys[idx] << " ";
			
		}
		_InOrder(pRoot->_pSub[idx]);
	}
	void _InsertKey(Node* pCur, const K& key, Node* pSub)
	{
		int end = pCur->_size - 1;
		while (end >= 0)
		{
			if (key < pCur->_keys[end])
			{
				pCur->_keys[end + 1] = pCur->_keys[end];
				pCur->_pSub[end + 2] = pCur->_pSub[end + 1];
				end--;
			}
			else
				break;
			
		}
		pCur->_keys[end+1] = key;
		pCur->_pSub[end + 2] = pSub;
		if (pSub)
			pSub->_pParent = pCur; 
		pCur->_size++;
	}

private:
	Node* _pRoot;
};


void TestBTree()
{
	BTree<int, 3> t;
	t.Insert(10);
	t.Insert(30);
	t.Insert(20);
	t.Insert(40);
	t.Insert(50);
	t.Insert(38);
	t.Insert(35);
	t.InOrder();
}




int main()
{
	TestBTree();
	return 0;
}