#include "iostream"
using namespace std;
#include "test.cpp"


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const T& weight)
	: _weight(weight)
	, _pLeft(NULL)
	, _pRight(NULL)
	, _pParent(NULL)
	{}

	T _weight;         // 权值
	HuffmanTreeNode<T>* _pLeft;
	HuffmanTreeNode<T>* _pRight;
	HuffmanTreeNode<T>* _pParent;
};


template<class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		: _pRoot(NULL)
	{}

	HuffmanTree(const T array[], size_t size, const T& invalid)
	{
		_Create(array, size, invalid);
	}


	~HuffmanTree()
	{
		_Destroy(_pRoot);
	}

	const Node* Root()const
	{
		return _pRoot;
	}



private:
	void _Create(const T array[], size_t size, const T& invalid)
	{
		struct Compare
		{
			bool operator()(const Node* left, const Node* right)
			{
				return left->_weight < right->_weight;
			}
		};
		Heap<Node*, Compare> _hp;//这里是保存节点指针如果为临时变量下次就不知道它的孩子双亲
		for (size_t i = 0; i < size; ++i)
		{
			if (array[i] != invalid)
				_hp.Insert(new Node(array[i]));
		}
			
		while (_hp.Size() != 1)
		{
			Node* Left = _hp.Top();
			_hp.Remove();
			Node* Right = _hp.Top();
			_hp.Remove();
			Node* parent = new Node(Left->_weight + Right->_weight);
			parent->_pLeft = Left;
			parent->_pRight = Right;
			Left->_pParent = parent;
			Right->_pParent = parent;
			_hp.Insert(parent);
		}
		_pRoot = _hp.Top();
	}
	void _Destroy(Node* & pRoot)
	{
		if (pRoot)
		{
			_Destroy(pRoot->_pLeft);
			_Destroy(pRoot->_pRight);
			delete pRoot;
			pRoot = NULL;
		}
	}

protected:
	Node* _pRoot;
};


int main()
{
	int array[] = {2,56,5,6,44,77};
	HuffmanTree<int> b1(array, sizeof(array)/sizeof(array[0]), 0);
	return 0;
}