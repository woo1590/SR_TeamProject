#include "EnginePCH.h"
#include "DecoratorNode.h"

DecoratorNode::DecoratorNode(BTNode* child)
{
}

DecoratorNode::~DecoratorNode()
{
}

void DecoratorNode::SetChild(BTNode* childNode)
{
	Child = childNode;
}

void DecoratorNode::Free()
{
	Safe_Delete(Child);
}
