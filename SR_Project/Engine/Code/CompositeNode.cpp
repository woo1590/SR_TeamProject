#include "EnginePCH.h"
#include "CompositeNode.h"

CompositeNode::CompositeNode()
{
}

CompositeNode::~CompositeNode()
{
}

void CompositeNode::AddChild(BTNode* child)
{
	if(child) ChildNodes.push_back(child);
}

void CompositeNode::Free()
{
	for (auto& Node : ChildNodes)
	{
		Safe_Release(Node);
	}
	ChildNodes.clear();
}
