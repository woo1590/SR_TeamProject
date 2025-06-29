#include "EnginePCH.h"
#include "BehaviorTree.h"

BehaviorTree::BehaviorTree(BTNode* rootNode)
	:RootNode(rootNode)
{
}

BehaviorTree::~BehaviorTree()
{
}

BehaviorTree* BehaviorTree::Create(BTNode* rootNode)
{
	BehaviorTree* Instance = new BehaviorTree(rootNode);
	return Instance;
}

void BehaviorTree::Tick(float dt, BlackBoard* bb)
{
	if (RootNode)
		RootNode->Tick(dt, bb);
}

void BehaviorTree::SetRoot(BTNode* rootNode)
{
	if (RootNode) Safe_Release(RootNode);
	RootNode = rootNode;
}

void BehaviorTree::Free()
{
	Safe_Release(RootNode);
}
