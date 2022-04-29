// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Algorithms/DijkstraGraphAlgorithm.h"
#include "Graph/GameGraphNode.h"
#include "Graph/GameGraph.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"
#pragma  optimize("",off)

/*
* Vertex additional info: currW, predecessor
Dijkstra(start)
	Init all vertices additional info
	Empty Processed Set
	Add all vertices to the queue
	while Q not empty
		u = extract min from Q
		insert u into processed
		for each v near u
			Relax(u,v,w)
 */

void UDijkstraGraphAlgorithm::Start(UGraphAlgorithmParams* InParams)
{
	//Init Params
	int32 StartIndex = -1;
	TargetIndex = -1;
	bFoundNode = false;
	USearchGraphAlgorithmParams* Params = Cast< USearchGraphAlgorithmParams>(InParams);
	if (Params)
	{
		StartIndex = Graph->GetNodeIndexByValue(Params->StartNode);
		TargetIndex = Graph->GetNodeIndexByValue(Params->TargetNode);
	}
	ensure(Graph->Nodes.IsValidIndex(StartIndex));
	int32 NodesNum = Graph->Nodes.Num();
	NodeDistances.Empty();
	NodeDistances.SetNum(NodesNum);

	NodePredecessors.Empty();
	NodePredecessors.SetNum(NodesNum);

	PQ.Empty();

	for (int32 i = 0; i < NodesNum; i++)
	{
		int32 dist = i == StartIndex ? 0 : TNumericLimits<int32>::Max();
		NodeDistances[i] = dist;
		NodePredecessors[i] = -1;
		PQ.Insert(i, dist);
		SetNodeAdditionalInfo(i);
	}

}

bool UDijkstraGraphAlgorithm::Step()
{
	if (PQ.IsEmpty())
		return true;
	int32 CurrNodeIndex = PQ.GetMin();
	PQ.PopMin();
	//if (CurrNodeIndex == TargetIndex)
	//{
	//	bFoundNode = true;
	//	return true;
	//}
	SetNodeProcessed(CurrNodeIndex);
	AGameGraphNode* CurrNode = Graph->Nodes[CurrNodeIndex];
	for (int32 NearNodeIndex : CurrNode->Edges)
	{
		Relax(CurrNodeIndex, NearNodeIndex);
	}
	return false;
}

void UDijkstraGraphAlgorithm::End()
{
// 	if (bFoundNode)
// 	{
		SetNodeFound(TargetIndex);
		int32 v = TargetIndex;
		int32 u = NodePredecessors[v];
		
		while (u != -1)
		{
			SetEdgeSelected(u, v);
			v = u;
			u = NodePredecessors[v];
		}
//	}
}

void UDijkstraGraphAlgorithm::SetEdgeSelected(int32 u, int32 v)
{
	Graph->GetEdge(u, v)->SetSelected();
}

void UDijkstraGraphAlgorithm::SetNodeProcessed(int32 NodeIndex)
{
	Graph->Nodes[NodeIndex]->SetColor(ProcessedColor);
}

void UDijkstraGraphAlgorithm::SetNodeFound(int32 NodeIndex)
{
	Graph->Nodes[NodeIndex]->SetColor(FoundColor);
}

void UDijkstraGraphAlgorithm::SetNodeAdditionalInfo(int32 NodeIndex)
{
	ensure(NodeDistances.IsValidIndex(NodeIndex) && NodePredecessors.IsValidIndex(NodeIndex));
	FString NodeDistance = NodeDistances[NodeIndex] == TNumericLimits<int32>::Max() ? "INF" : FString::FromInt(NodeDistances[NodeIndex] / 1000.f);
	FString NodeInfo = "D: " + NodeDistance  + " P: " + FString::FromInt(NodePredecessors[NodeIndex]);
	Graph->Nodes[NodeIndex]->SetAdditionalText(NodeInfo);
}

void UDijkstraGraphAlgorithm::Relax(int32 u, int32 v)
{

	int32 w = Graph->GetEdge(u,v)->GetWeight() * 1000.f;
	if (NodeDistances[v] > NodeDistances[u] + w)
	{
		NodeDistances[v] = NodeDistances[u] + w;
		NodePredecessors[v] = u;
		SetNodeAdditionalInfo(v);
		PQ.DecreasePriority(v, NodeDistances[v]);
	}
}
