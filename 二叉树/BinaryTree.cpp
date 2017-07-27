#include "iostream"
using namespace std;
#include <queue>
#include <assert.h>
#include <stack>


template <class T>
struct BinaryTreeNode
{
	T _value;
	BinaryTreeNode<T> *_Tleft;
	BinaryTreeNode<T> *_Tright;
	BinaryTreeNode(T value) :
		_value(value),
		_Tleft(NULL),
		_Tright(NULL)
	{}

};

template <class T>
class BinaryTree
{
public:
	typedef BinaryTreeNode<T> Node;
private:
	Node *_pRoot;
public:
	BinaryTree() :_pRoot(NULL) //构造函数时一定要给成员变量初始化。避免以后的问题
	{}
	BinaryTree(T array[], size_t size, T invalid)
	{
		size_t index = 0;
		CreatBinaryTree(array, size, _pRoot, invalid, index);
	}

	BinaryTree(const BinaryTree<T>& bt)
	{
		_pRoot = _CopyBinaryTree(bt._pRoot);
	}

	BinaryTree<T>& operator=(const BinaryTree<T>&bt)
	{
		if (this != &bt)
		{
			DestoryBinaryTree(_pRoot);
			_pRoot = _CopyBinaryTree(bt._pRoot);
		}
		return *this;
	}

	~BinaryTree()
	{
		DestoryBinaryTree(_pRoot);
	}

	void PreOrder()
	{
		cout << "PreOrder:" << endl;
		_PreOrder(_pRoot);
		cout << endl;
	}

	void PreOrder_Nor()
	{
		cout << "PreOrder_Nor:" << endl;
		_PreOrder_Nor(_pRoot);
		cout << endl;
	}

	void Inorder()
	{
		cout << "Inorder:" << endl;
		_Inorder(_pRoot);
		cout << endl;
	}

	void Inorder_Nor()
	{
		cout << "Inorder_Nor:" << endl;
		_Inorder_Nor(_pRoot);
		cout << endl;
	}

	void PostOrder()
	{
		cout << "PostOrder:" << endl;
		_PostOrder(_pRoot);
		cout << endl;
	}
	void PostOrder_Nor()
	{
		cout << "PostOrder_Nor:" << endl;
		_PostOrder_Nor(_pRoot);
		cout << endl;
	}

	void LevelOrder()
	{
		cout << "LevelOrder:" << endl;
		_LevelOrder(_pRoot);
		cout << endl;
	}
	

	Node* Find(const T& value)
	{
		Node* pCur = _Find(_pRoot, value);//创建临时变量利于测试
		return pCur;
	}

	Node* Parent(Node* pCur)
	{
		Node* temp = _Parent(_pRoot, pCur);//创建临时变量利于测试
		return temp;
	}

	Node* GetLeftChild(Node* pCur)
	{
		return (pCur == NULL) ? NULL : pCur->_pLeft;
	}

	Node* GetRightChild(Node* pCur)
	{
		return (pCur == NULL) ? NULL : pCur->_pRight;
	}

	size_t Height()
	{
		size_t temp = _Height(_pRoot);
		return temp;
	}

	size_t GetLeefCount()
	{
		size_t i = _GetLeefCount(_pRoot);
		return i;
	}

	size_t GetKLevelCount(size_t k)
	{
		size_t i = _GetKLevelCount(_pRoot, k);
		return i;
	}


	void BinaryMirror()
	{
		_BinaryMirror(_pRoot);
	}

	void BinaryMirror_Nor()
	{
		_BinaryMirror_Nor(_pRoot);
	}

	bool IsCompleteBinaryTree()
	{
		return _IsCompleteBinaryTree(_pRoot);
	}


private:
	void CreatBinaryTree(T array[], size_t size, Node *&pRoot, T invalid, size_t &index)
	{
		if (index < size && invalid != array[index])
		{
			pRoot = new Node(array[index]);
			
			CreatBinaryTree(array, size, pRoot->_Tleft, invalid, ++index);
			CreatBinaryTree(array, size, pRoot->_Tright, invalid, ++index);
		}
	}

	Node* _CopyBinaryTree(Node *pRoot)
	{
		Node* pCur = NULL;
		if (pRoot)
		{
			Node* lchild  = _CopyBinaryTree(pRoot->_Tleft);
			Node* rchild = _CopyBinaryTree(pRoot->_Tright);
			pCur = new Node(pRoot->_value);
			pCur->_value = pRoot->_value;
			pCur->_Tleft = lchild;
			pCur->_Tright = rchild;
		}
		return pCur;
	}

	void DestoryBinaryTree(Node *&pRoot)
	{
		if (pRoot)
		{
			DestoryBinaryTree(pRoot->_Tleft);
			DestoryBinaryTree(pRoot->_Tright);
			delete pRoot;
			pRoot = NULL;
		}
	}

	void _PreOrder(Node *pRoot) //遍历的操作在这里定为打印，前序遍历
	{
		if (pRoot)
		{
			cout << pRoot->_value << " ";
			_PreOrder(pRoot->_Tleft);
			_PreOrder(pRoot->_Tright);
		}
	}

	//直接访问左子树遇到有右子树的在保存
	//void _PreOrder_Nor(Node* pRoot)
	//{
	//	if (pRoot == NULL)
	//		return;
	//	stack<Node*> s;
	//	s.push(pRoot);
	//	Node* pCur = NULL;
	//	while (!s.empty())
	//	{	
	//		Node* pCur = s.top();
	//		s.pop();
	//		while (pCur)
	//		{
	//			cout << pCur->_value << " "; 
	//			if (pCur->_Tright)
	//			{
	//				s.push(pCur->_Tright);
	//			}
	//			pCur = pCur->_Tleft;
	//		}
	//	}
	//}

	void _PreOrder_Nor(Node* pRoot)
	{
		if (pRoot == NULL)
			return;
		stack<Node*> s;
		s.push(pRoot);
		Node* pCur = NULL;
		while (!s.empty())
		{
			Node* pCur = s.top();
			cout << pCur->_value << " ";
			s.pop();
			if (pCur->_Tright)
				s.push(pCur->_Tright);
			if (pCur->_Tleft)
				s.push(pCur->_Tleft);
		}
	}

	void _Inorder(Node *pRoot)//中序遍历
	{
		if (pRoot)
		{
			_Inorder(pRoot->_Tleft);
			cout << pRoot->_value << " ";
			_Inorder(pRoot->_Tright);
		}
	}

	//方法一、
	//void _Inorder_Nor(Node* pRoot)
	//{
	//	if (pRoot == NULL)
	//		return;
	//	Node* pCur = pRoot;
	//	stack<Node*> s;
	//	while (pCur || !s.empty())
	//	{
	//		while (pCur)
	//		{
	//			s.push(pCur);
	//			pCur = pCur->_Tleft;
	//		}
	//		pCur = s.top();
	//		cout << pCur->_value << " ";
	//		s.pop();
	//		pCur = pCur->_Tright;
	//	}
	//}

	//方法二、
	void _Inorder_Nor(Node* pRoot)
	{
		if (pRoot == NULL)
			return;
		Node* pCur = pRoot;
		stack<Node*> s;
		while (pCur || !s.empty())
		{
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_Tleft;
			}
			pCur = s.top();
			while (NULL == pCur->_Tright && !s.empty())
			{
				pCur = s.top();
				cout << pCur->_value << " ";
				s.pop();
			}
			pCur = pCur->_Tright;
		}
	}


	void _PostOrder(Node *pRoot)//后序遍历
	{
		if (pRoot)
		{
			_PostOrder(pRoot->_Tleft);
			_PostOrder(pRoot->_Tright);
			cout << pRoot->_value << " ";
		}
	}
	
	void _PostOrder_Nor(Node* pRoot)
	{
		if (pRoot == NULL)
			return;
		Node* pCur = pRoot;
		Node* prev = NULL;
		Node* pTop = NULL;
		stack<Node*> s;
		while (pCur || !s.empty())
		{
			while (pCur)
			{
				s.push(pCur);
				pCur = pCur->_Tleft;
			}
			pTop = s.top();
			if (pTop->_Tright == NULL || pTop->_Tright == prev)//
			{
				cout << pTop->_value << " ";
				s.pop();
				prev = pTop;
			}
			else
				pCur = pTop->_Tright;
		}
	}
	void _LevelOrder(Node *pRoot)
	{
		if (pRoot == NULL)
			return;
		queue<Node*> q;
		q.push(pRoot);
		Node* pCur = NULL;
		while (!q.empty())
		{
			pCur = q.front();
			cout << pCur->_value << " ";
			if (pCur->_Tleft)
				q.push(pCur->_Tleft);
			if (pCur->_Tright)
				q.push(pCur->_Tright);
			q.pop();
		}
	}
	
	Node* _Find(Node* pRoot, const T& value)
	{
		if (pRoot == NULL)
			return NULL;
		if (pRoot->_value == value)
			return pRoot;
		Node* pCur = NULL;
		if (pCur = _Find(pRoot->_Tleft, value))
			return pCur;
		return _Find(pRoot->_Tright, value);
	}

	Node* _Parent(Node* pRoot, Node* pCur)
	{
		assert(pCur);
		if (pRoot == NULL || pCur == pRoot)
			return NULL;
		if (pRoot->_Tleft == pCur || pRoot->_Tright == pCur)
			return pRoot;
		Node* temp = NULL;
		if (temp = _Parent(pRoot->_Tleft, pCur))
			return temp;
		if (temp = _Parent(pRoot->_Tright, pCur))
			return temp;
		return NULL;
	}

	size_t _Height(Node* pRoot)
	{
		if (pRoot == NULL)
			return 0;
		if (pRoot->_Tleft == NULL && pRoot->_Tright == NULL)
			return 1;
		size_t LeftHeight = _Height(pRoot->_Tleft);
		size_t RightHeight = _Height(pRoot->_Tright);
		return (pRoot->_Tleft > pRoot->_Tright) ? LeftHeight + 1 : RightHeight + 1;
	}

	size_t _GetLeefCount(Node* pRoot)
	{
		if (pRoot == NULL)
			return 0;
		if (pRoot->_Tleft == NULL && pRoot->_Tright == NULL)
			return 1;
		return _GetLeefCount(pRoot->_Tleft) + _GetLeefCount(pRoot->_Tright);
	}

	size_t _GetKLevelCount(Node* pRoot, size_t k)
	{
		if (pRoot == NULL || k < 1)
			return 0;
		if (k == 1)
			return 1;
		size_t a = _GetKLevelCount(pRoot->_Tleft, k - 1);
		size_t b = _GetKLevelCount(pRoot->_Tright, k - 1);
		return a + b;
	}

	Node* _BinaryMirror_Nor(Node* pRoot)
	{
		if (pRoot == NULL)
			return NULL;
		queue<Node*> q;
		q.push(pRoot);
		Node* pCur = NULL;
		while (!q.empty())
		{
			pCur = q.front();
			q.pop();
			if (pCur->_Tleft)
				q.push(pCur->_Tleft);
			if (pCur->_Tright)
				q.push(pCur->_Tright);
			std::swap(pCur->_Tleft, pCur->_Tright);
		}
		return pRoot;
	}

	Node* _BinaryMirror(Node* pRoot)
	{
		if (pRoot)
		{
			std::swap(pRoot->_Tleft, pRoot->_Tright);
			_BinaryMirror(pRoot->_Tleft);
			_BinaryMirror(pRoot->_Tright);
		}
		return pRoot;
	}
	
	bool _IsCompleteBinaryTree(Node* _pRoot)
	{
		queue<Node*> q;
		q.push(_pRoot);
		bool flag = false;
		while (!q.empty())
		{
			Node* pCur = q.front();
			q.pop();
			if (flag)
			{
				if (pCur->_Tleft || pCur->_Tright)
					return false;
			}
			else
			{
				if (pCur->_Tleft && pCur->_Tright)
				{
					q.push(pCur->_Tleft);
					q.push(pCur->_Tright);
				}
				else if (pCur->_Tleft)
					flag = true;
				else if (pCur->_Tright)
					return false;
				else if (pCur->_Tleft == NULL && pCur->_Tright == NULL)
					flag = true;
			}
		}
		return flag;
	}
};

int main()
{
	//char* pStr = "124#78##9###35##6";
	char* pStr = "124###35##6";
	BinaryTree<char> b1(pStr,strlen(pStr), '#');
	BinaryTree<char> b2(b1);
	BinaryTree<char> b3;
	b3 = b1;
	//b1.PreOrder();
	//b1.Inorder();
	//b1.PostOrder();
	//b1.LevelOrder();
	//b1.Find('3');
	//b1.Parent(b1.Find('4'));
	//size_t i = b1.Height();
	//size_t i = b1.GetLeefCount();
	//size_t i = b1.GetKLevelCount(3);
	//b1.BinaryMirror();
	//b1.BinaryMirror_Nor();
	//b1.PreOrder_Nor();
	//b1.Inorder_Nor();
	b1.PostOrder_Nor();
	//bool flag = b1.IsCompleteBinaryTree();
	return 0;
}