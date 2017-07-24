#include <iostream>
using namespace std;


enum PointIhfo{ LINK, THREAD };

template<class T>
struct BinaryTreeNodeThd
{
	T _data;
	BinaryTreeNodeThd<T>* _pLeft;
	BinaryTreeNodeThd<T>* _pRight;
	BinaryTreeNodeThd<T>* _pParent;
	PointIhfo _leftThread;
	PointIhfo _rightTrhead;

	BinaryTreeNodeThd(const T& data)
		: _data(data)
		, _pLeft(NULL)
		, _pRight(NULL)
		, _pParent(NULL)
		, _leftThread(LINK)
		, _rightTrhead(LINK)
	{}
};


template<class T>
class BinaryTreeThd
{
	typedef BinaryTreeNodeThd<T> Node;
public:
	BinaryTreeThd()
		: _pRoot(NULL)
	{}

	BinaryTreeThd(const T array[], size_t size, const T& invalid)
	{
		size_t index = 0;
		_CreateTree(_pRoot, array, size, index, invalid);
	}

	void PreThread()
	{
		Node* prev = NULL;
		_PreThread(_pRoot, prev);
	}

	void InThread()
	{
		Node* prev = NULL;
		_InThread(_pRoot, prev);
	}

	void PostThread()
	{
		Node* prev = NULL;
		_PostThread(_pRoot, prev);
	}

	void PreOrder()
	{
		_PreOrder(_pRoot);
	}
	void InOrder()
	{
		_InOrder(_pRoot);
	}
	void PostOrder();

	

private:
	
	void _CreateTree(Node*& pRoot, const T array[], size_t size,
		size_t& index, const T& invalid)
	{
		if (index < size && invalid != array[index])
		{
			pRoot = new Node(array[index]);
			_CreateTree(pRoot->_pLeft, array, size, ++index, invalid);
			if (pRoot->_pLeft)
				pRoot->_pLeft->_pParent = pRoot;
			_CreateTree(pRoot->_pRight, array, size, ++index, invalid);
			if (pRoot->_pRight)
				pRoot->_pRight->_pParent = pRoot;
		}
	}

	// 前序线索化
	void _PreThread(Node* pRoot, Node* &prev)
	{
		if (pRoot)
		{
			//线索化左指针域
			if (pRoot->_pLeft == NULL)
			{
				pRoot->_pLeft = prev;
				pRoot->_leftThread = THREAD;
			}
			//线索化右指针域
			if (prev != NULL && prev->_pRight == NULL)
			{
				prev->_pRight = pRoot;
				prev->_rightTrhead = THREAD;
			}
			//保存当前节点
			prev = pRoot;
			//线索化左子树
			if (pRoot->_leftThread == LINK)
				_PreThread(pRoot->_pLeft, prev);
			//线索化又子树
			if (pRoot->_rightTrhead == LINK)
				_PreThread(pRoot->_pRight, prev);
		}
	}

	// 中序线索化
	void _InThread(Node* pRoot, Node* &prev)
	{
		if (pRoot)
		{
			//线索化左子树
			_InThread(pRoot->_pLeft, prev);
			//线索化左指针域
			if (pRoot->_pLeft == NULL)
			{
				pRoot->_pLeft = prev;
				pRoot->_leftThread = THREAD;
			}
			//线索化右指针域
			if (prev != NULL && prev->_pRight == NULL)
			{
				prev->_pRight = pRoot;
				prev->_rightTrhead = THREAD;
			}
			//保存当前节点
			prev = pRoot;

			//线索化又子树
			if (pRoot->_rightTrhead == LINK)
				_InThread(pRoot->_pRight, prev);
		}
	}

	// 后续线索化
	void _PostThread(Node* pRoot, Node* &prev)
	{
		if (pRoot)
		{
			//线索化左子树
			_PostThread(pRoot->_pLeft, prev);
			//线索化又子树
			if (pRoot->_rightTrhead == LINK)
				_PostThread(pRoot->_pRight, prev);
			//线索化左指针域
			if (pRoot->_pLeft == NULL)
			{
				pRoot->_pLeft = prev;
				pRoot->_leftThread = THREAD;
			}
			//线索化右指针域
			if (prev != NULL && prev->_pRight == NULL)
			{
				prev->_pRight = pRoot;
				prev->_rightTrhead = THREAD;
			}
			//保存当前节点
			prev = pRoot;
		}
	}

	void _PreOrder(Node* pRoot)
	{
		if (pRoot == NULL)
			return;
		Node* pCur = pRoot;
		while (pCur)
		{
			while (pCur->_leftThread == LINK)
			{
				cout << pCur->_data << " ";
				pCur = pCur->_pLeft;
			}

			cout << pCur->_data <<" ";
			pCur = pCur->_pRight;
			//while (pCur && pCur->_rightTrhead == THREAD)
			//{
			//	pCur = pCur->_pRight;
			//	cout << pCur->_data << " ";
			//}
			//if (pCur->_leftThread == LINK)
			//{
			//	pCur = pCur->_pLeft;
			//}
			//else
			//	pCur = pCur->_pRight;
		}
	}

	void _InOrder(Node* pRoot)
	{
		if (pRoot == NULL)
			return;
		Node* pCur = pRoot;
		while (pCur)
		{
			while (pCur->_leftThread == LINK)
				pCur = pCur->_pLeft;

			cout << pCur->_data << " ";
			while (pCur && pCur->_rightTrhead == THREAD)
			{
				pCur = pCur->_pRight;
				cout << pCur->_data << " ";
			}
			pCur = pCur->_pRight;
		}
	}
protected:
	Node* _pRoot;
};

int main()
{
	char str[] = "124#78##9###35##6";
	BinaryTreeThd<char> b(str, strlen(str), '#');
	//b.PreThread();
	//b.PreOrder();
	b.InThread();
	b.InOrder();
	//b.PostThread();
	return 0;
}