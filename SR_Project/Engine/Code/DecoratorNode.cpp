#include "EnginePCH.h"
#include "DecoratorNode.h"

DecoratorNode::DecoratorNode(BTNode* child)
	:Child(child)
{
}

DecoratorNode::~DecoratorNode()
{
}

BTStatus DecoratorNode::Initialize(float dt)
{
	if (Child)
		return Child->Initialize(dt);
	return BTStatus::Failure;
}

void DecoratorNode::SetChild(BTNode* childNode)
{
	Child = childNode;
}

void DecoratorNode::Free()
{
	Safe_Release(Child);
}
