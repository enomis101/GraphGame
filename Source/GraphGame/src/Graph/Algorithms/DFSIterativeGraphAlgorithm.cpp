// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Algorithms/DFSIterativeGraphAlgorithm.h"
#include "Graph/GameGraphNode.h"
#include "Graph/GameGraph.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"

/*
* PSEUDOCODE
bool DFS(Node root, Node target) {
	unordered_set<Node> visited;
	stack<Node> stack;
	//add root to stack
		stack.push(root);
	while (!stack.empty()) {
		Node cur = stack.top();
		stack.pop();
		if(curr == target)
					return true;

		for (Node next : getNear(cur)) {
			if (visited.count(next) <= 0) {
				visited.insert(next);
				stack.push(next);
			}
		}
	}
	return false;
}
 */

void UDFSIterativeGraphAlgorithm::Start(UGraphAlgorithmParams* InParams)
{
	//Init Params
	AGameGraphNode* RootNode = nullptr;
	Target = -1;
	USearchGraphAlgorithmParams* Params = Cast< USearchGraphAlgorithmParams>(InParams);
	if (Params)
	{
		RootNode = Graph->GetNodeAtIndex(Params->StartNode, true /*bFallbackLastClicked*/);
		Target = Params->TargetNode;
	}

	if (!RootNode)
	{
		return;
	}

	Stack.Empty();
	ExploredNodes.Empty();
	FoundNode = nullptr;

	//Start Algorithm
	Stack.Add(RootNode);
	SetNodeExplored(RootNode);
}

bool UDFSIterativeGraphAlgorithm::Step()
{
	if (Stack.Num() != 0)
	{
		int32 Top = Stack.Num() - 1;
		AGameGraphNode* CurrNode = Stack[Top];
		Stack.RemoveAt(Top);
		ensure(CurrNode);

		if (CurrNode->GetId() == Target)
		{
			FoundNode = CurrNode;
			return true;
		}
		else
		{
			CurrNode->SetColor(ProcessedColor);
		}
		int32 LastIndex = CurrNode->Edges.Num() - 1;
		for (int32 i = LastIndex;  i >= 0; i--)
		{
			int32 AdjIndex = CurrNode->Edges[i];
			AGameGraphNode* AdjNode = Graph->Nodes[AdjIndex];
			ensure(AdjNode);
			if (!ExploredNodes.Contains(AdjNode))
			{
				SetNodeExplored(AdjNode);
				Stack.Add(AdjNode);
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}

void UDFSIterativeGraphAlgorithm::End()
{
	if (FoundNode)
	{
		FoundNode->SetColor(FoundColor);
	}
	else
	{

	}
}

void UDFSIterativeGraphAlgorithm::SetNodeExplored(AGameGraphNode* InNode)
{
	ExploredNodes.Add(InNode);
	InNode->SetColor(ExploredColor);
}
