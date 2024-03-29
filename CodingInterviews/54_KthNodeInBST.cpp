#include <cstdio>
#include "BinaryTree.h"

const BinaryTreeNode* KthNodeCore(const BinaryTreeNode* pRoot, unsigned int& k);

const BinaryTreeNode* KthNode(const BinaryTreeNode* pRoot, unsigned int k)
{
	if (pRoot == nullptr || k == 0)
		return nullptr;

	return KthNodeCore(pRoot, k);
}

const BinaryTreeNode* KthNodeCore(const BinaryTreeNode* pRoot, unsigned int& k)
{
	const BinaryTreeNode* target = nullptr;

	if (pRoot->m_pLeft != nullptr)
		target = KthNodeCore(pRoot->m_pLeft, k);

	if (target == nullptr)
	{
		if (k == 1)
			target = pRoot;

		k--;
	}

	if (target == nullptr && pRoot->m_pRight != nullptr)
		target = KthNodeCore(pRoot->m_pRight, k);

	return target;
}