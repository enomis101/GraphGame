// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Algorithms/BFSGraphAlgorithm.h"
#include "Graph/GameGraphNode.h"
#include "Graph/GameGraph.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"

/*
* PSEUDOCODE
 1  procedure BFS(G, root) is
 2      let Q be a queue
 3      label root as explored
 4      Q.enqueue(root)
 5      while Q is not empty do
 6          v := Q.dequeue()
 7          if v is the goal then
 8              return v
 9          for all edges from v to w in G.adjacentEdges(v) do
10              if w is not labeled as explored then
11                  label w as explored
12                  Q.enqueue(w)
 */

void UBFSGraphAlgorithm::Start(UGraphAlgorithmParams* InParams)
{
	//Init Params
	UGraphNode* RootNode = nullptr;
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

	Queue.Empty();
	ExploredNodes.Empty();
	//ProcessedNodes.Empty();
	FoundNode = nullptr;

	//Start Algorithm
	Queue.Enqueue(RootNode);
	ExploredNodes.Add(RootNode);
	RootNode->SetColor(ExploredColor);
}

bool UBFSGraphAlgorithm::Step()
{
	if (!Queue.IsEmpty())
	{
		UGraphNode* CurrNode;

		ensure(Queue.Dequeue(CurrNode));
		ensure(CurrNode);

		if (CurrNode->GetId() == Target)
		{
			FoundNode = CurrNode;
			return true;
		}
		else
		{
			//ProcessedNodes.Add(CurrNode);
			CurrNode->SetColor(ProcessedColor);
		}
		for (int32 NearIndex : CurrNode->Edges)
		{
			UGraphNode* AdjNode = Graph->Nodes[NearIndex];
			ensure(AdjNode);
			if (!ExploredNodes.Contains(AdjNode))
			{
				ExploredNodes.Add(AdjNode);
				AdjNode->SetColor(ExploredColor);
				Queue.Enqueue(AdjNode);
			}
		}
		return false;
	}
	else
	{
		return true;
	}
}

void UBFSGraphAlgorithm::End()
{
	if (FoundNode)
	{
		FoundNode->SetColor(FoundColor);
	}
	else
	{

	}
}
