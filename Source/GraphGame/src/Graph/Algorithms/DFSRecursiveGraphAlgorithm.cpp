// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/Algorithms/DFSRecursiveGraphAlgorithm.h"
#include "Graph/GameGraphNode.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"
#include "Graph/GameGraph.h"
#pragma optimize("",off)

void UDFSRecursiveObject::Init(AGameGraphNode* InRootNode)
{
	RootNode = InRootNode;
	CheckPoint = 0;
	Algorithm = Cast< UDFSRecursiveGraphAlgorithm>(GetOuter());
}

/*
A recursive implementation of DFS:[5]

procedure DFS(G, v) is
    label v as discovered
    for all directed edges from v to w that are in G.adjacentEdges(v) do
        if vertex w is not labeled as discovered then
            recursively call DFS(G, w)
*/
bool UDFSRecursiveObject::Step()
{
	if (CheckPoint == 0)
	{
		Algorithm->SetNodeProcessed(RootNode);
		if (RootNode->GetId() == Algorithm->Target)
		{
			Algorithm->SetNodeFound(RootNode);
			return true;
		}
		CheckPoint = 1;
		i = 0;
		return false;
	}

	if (CheckPoint == 1)
	{
		while (i < RootNode->Edges.Num())
		{
			int32 NearIndex = RootNode->Edges[i];
			AGameGraphNode* NearNode = Algorithm->Graph->Nodes[NearIndex];
			i++;
			if (!Algorithm->IsNodeExplored(NearNode))
			{
				Algorithm->SetNodeExplored(NearNode);
				Recurse(NearNode);
				return false;
			}
		}
		CheckPoint = 2;
	}
	return false;
}

void UDFSRecursiveObject::BeginDestroy()
{
	Super::BeginDestroy();

}

void UDFSRecursiveObject::Recurse(AGameGraphNode* InRootNode)
{
	UDFSRecursiveObject* NewRecObject = NewObject< UDFSRecursiveObject>(Algorithm, UDFSRecursiveObject::StaticClass());
	if (NewRecObject)
	{
		NewRecObject->Init(InRootNode);
		Algorithm->Recurse(NewRecObject);
	}
}

//Algorithm
void UDFSRecursiveGraphAlgorithm::Start(UGraphAlgorithmParams* InParams)
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
	//ProcessedNodes.Empty();
	FoundNode = nullptr;

	//Start Algorithm
	UDFSRecursiveObject* NewRecObject = NewObject<UDFSRecursiveObject>(this, UDFSRecursiveObject::StaticClass());
	if (NewRecObject)
	{
		NewRecObject->Init(RootNode);
		SetNodeExplored(RootNode);
		//Start by pushing the first function on the stack
		Recurse(NewRecObject);
	}
}

void UDFSRecursiveGraphAlgorithm::End()
{
	if (FoundNode)
	{
		FoundNode->SetColor(FoundColor);
	}
	for (int i = 0; i < Stack.Num(); i++)
	{
		Stack[i]->ConditionalBeginDestroy();
		Stack[i] = nullptr;
	}
	Stack.Empty();
}

bool UDFSRecursiveGraphAlgorithm::IsNodeExplored(AGameGraphNode* InNode)
{
	return ExploredNodes.Contains(InNode);
}

void UDFSRecursiveGraphAlgorithm::SetNodeExplored(AGameGraphNode* InNode)
{
	ExploredNodes.Add(InNode);
	InNode->SetColor(ExploredColor);
}

void UDFSRecursiveGraphAlgorithm::SetNodeProcessed(AGameGraphNode* InNode)
{
	InNode->SetColor(ProcessedColor);
}

void UDFSRecursiveGraphAlgorithm::SetNodeFound(AGameGraphNode* InNode)
{
	FoundNode = InNode;
}
