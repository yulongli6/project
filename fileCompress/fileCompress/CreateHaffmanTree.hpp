#pragma once
#include"FileCompressHaffman.h"
#include<queue>

template<class T>
struct HaffmanTreeNode
{
	HaffmanTreeNode(const T& val)
	:_val(val)
	, pLeft(nullptr)
	, pRight(nullptr)
	, pParent(nullptr)
	{}
	T _val;
	HaffmanTreeNode<T>* pLeft;
	HaffmanTreeNode<T>* pRight;
	HaffmanTreeNode<T>* pParent;
};

template<class T>
class CreateHaffmanTree
{
	typedef HaffmanTreeNode<T> Node;
	typedef Node* pNode;
public:

	CreateHaffmanTree()
		:pRoot(nullptr)
	{}

	void GetHaffmanTree(std::vector<T>& v)
	{
		if (v.empty())
			return;
		std::priority_queue < pNode, std::vector<pNode>, cmp> q;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i]._count != 0)
			{
				pNode node = new Node(v[i]);
				q.push(node);
			}
		}

		while (q.size() > 1)
		{

			pNode left = q.top();
			q.pop();
			pNode right = q.top();
			q.pop();

			pNode parent = new Node(left->_val + right->_val);

			parent->pLeft = left;
			parent->pRight = right;
			left->pParent = parent;
			right->pParent = parent;

			q.push(parent);
		}
		pRoot = q.top();
	}

	~CreateHaffmanTree()
	{
		Destroy(pRoot);
	}

	pNode GetRoot()
	{
		return pRoot;
	}

	struct cmp
	{
		bool operator()(pNode& left, pNode& right)
		{
			return left->_val._count > right->_val._count;
		}
	};
private:
	void Destroy(pNode root)
	{
		if (nullptr == root)
			return;

		Destroy(root->pLeft);
		Destroy(root->pRight);
		delete root;
		root = nullptr;
	}
private:
	pNode pRoot;
};






