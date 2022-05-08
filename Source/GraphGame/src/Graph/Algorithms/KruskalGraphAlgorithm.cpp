// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Algorithms/KruskalGraphAlgorithm.h"
#include "Graph/GameGraphNode.h"
#include "Graph/GameGraph.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"
#include "Graph/GraphFunctionLibrary.h"
#include "../../../include/Graph/GameGraphEdge.h"

/*
* PSEUDOCODE
1 MST = 0
2 UnionFInd U(Nodes.size())
3 sort edges by w
4 for each sorted edge x,y
5 	if(!U.AreConnected(x,y))
6		MST.Add(x,y)
7		U.Union(x,y)
 */

void UMSTKruskalGraphAlgorithm::Start(UGraphAlgorithmParams* InParams)
{
	bValidGraph = Graph && !Graph->IsDirected() && Graph->Nodes.Num() > 0 &&  UGraphFunctionLibrary::IsGraphConnected(Graph);
	if (!ensure(bValidGraph))
	{
		return;
	}

	U.Empty();
	U.Init(Graph->Nodes.Num());
	MST.Empty();
	SortedEdges.Empty();

	SortedEdges = Graph->Edges;
	SortedEdges.Sort([](const UGraphEdge& a, const UGraphEdge& b) { return a.GetWeight() < b.GetWeight(); });
	i = 0; 
}

bool UMSTKruskalGraphAlgorithm::Step()
{
	if (!bValidGraph)
	{
		//Finish early
		return true;
	}
	if (i < SortedEdges.Num())
	{
		UGraphEdge* CurrEdge = SortedEdges[i];

		if (!U.AreConnected(CurrEdge->Node1, CurrEdge->Node2))
		{
			AddEdge(CurrEdge);
		}
		else
		{
			CurrEdge->SetColor(NotMSTEdgeColor);
		}

		i++;
		return false;
	}
	return true;
}

void UMSTKruskalGraphAlgorithm::End()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Kruskal Finished");
}

void UMSTKruskalGraphAlgorithm::AddEdge(UGraphEdge* EdgeToAdd)
{
	MST.Add(EdgeToAdd);
	U.Union(EdgeToAdd->Node1, EdgeToAdd->Node2);

	EdgeToAdd->SetColor(MSTEdgeColor);
	UGraphNode* FirstNode = Graph->Nodes[EdgeToAdd->Node1];
	UGraphNode* SecondNode = Graph->Nodes[EdgeToAdd->Node2];
	FirstNode->SetColor(MSTNodeColor);
	SecondNode->SetColor(MSTNodeColor);
}
