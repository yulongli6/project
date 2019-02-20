#pragma once
#include"FileCompressHaffman.h"
#include<vector>
#include<queue>

template<class T>
struct HaffmanTreeNode
{
	HaffmanTreeNode(T val)
	:_val(val)
	, pLeft(nullptr)
	, pRight(nullptr)
	, pParent(nullptr)
	{}
	T _val;
	HaffmanTreeNode* pLeft;
	HaffmanTreeNode* pRight;
	HaffmanTreeNode* pParent;
};

template<class T>
class CreateHaffmanTree
{
public:
	typedef HaffmanTreeNode<T> Node;
	typedef Node* pNode;
	CreateHaffmanTree()
		:pLeft(nullptr)
		,pRight(nullptr)
		,pParent(nullptr)
	{}

	void GetHaffmanTree(std::vector<T>& v)
	{
		std::priority_queue < pNode, std::vector<pNode>, cmp> q;
		for (size_t i = 0; i < v.size(); ++i)
		{
			if (v[i]._count != 0)
			{
				pNode * node = new Node(v[i]);
				q.push(node);
			}
		}

		pNode parent = nullptr;
		while (q.size() > 1)
		{
			pNode left = q.top;
			q.pop();
			pNode right = q.top;
			q.pop();
			parent = new Node(left->_val + right->_val);
			parent->pLeft = left;
			parent->right = right;
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

		Destory(root->_pLeft);
		Destory(root->_pRight);
		delete root;
		root = nullptr;
	}
	pNode pRoot;
};





