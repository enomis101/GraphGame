// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/Algorithms/PrimGraphAlgorithm.h"
#include "Graph/GameGraphNode.h"
#include "Graph/GameGraph.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"
#include "Graph/GraphFunctionLibrary.h"
#include "Graph/GameGraphEdge.h"

/*
* PSEUDOCODE
1 Init Set Visited
2 Add To Q each vertex with priority = root ? 0 : INF 
3 while !Q.IsEmpty()
4 curr = Q.ExtractMin
5 for each v Adj to u
6	if !Visited.Contains(v) 
7		Q.DecreasePriority(v, Edge(u,v).w)
 */

void UMSTPrimGraphAlgorithm::Start(UGraphAlgorithmParams* InParams)
{
	bValidGraph = Graph && !Graph->IsDirected() && Graph->Nodes.Num() > 0 &&  UGraphFunctionLibrary::IsGraphConnected(Graph);
	if (!ensure(bValidGraph))
	{
		return;
	}

	Q.Empty();
	NMST.Empty();
	Root = 0;
	//Add every node to the Queue with priority INF if not Root and 0 if it's
	//Init Parents node
	for (int32 i = 0; i < Graph->Nodes.Num(); i++)
	{
		int32 Priority = (i == Root) ? 0 : TNumericLimits<int32>::Max();
		Q.Insert(i, Priority);
		Parents.Add(-1);
	}
}

bool UMSTPrimGraphAlgorithm::Step()
{
	if (!bValidGraph)
	{
		//Finish early
		return true;
	}
	if (!Q.IsEmpty())
	{
		int32 Curr = Q.GetMin();
		Q.PopMin();
		AddToMST(Curr);
		//For each node v adj to Curr, if it is not in MST (so it must be in Q, quicker this way)
		//Relax the edge Curr, v, decreasing the priority of v if edge (u,v) weight has lesser priority than v's curr priority
		for (int32 v : Graph->Nodes[Curr]->Edges)
		{
			if (!NMST.Contains(v))
			{
				Relax(Curr, v);
			}
		}

		return false;
	}
	return true;
}

void UMSTPrimGraphAlgorithm::End()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, "Prim Finished");
	//Mark all the edge not in the MST of this color
	for (UGraphEdge* Edge : Graph->Edges)
	{
		if (!EMST.Contains(Edge))
		{
			Edge->SetColor(NotMSTEdgeColor);
		}
	}
}

void UMSTPrimGraphAlgorithm::AddToMST(int32 NodeIndex)
{
	NMST.Add(NodeIndex);
	UGraphNode* Node = Graph->Nodes[NodeIndex];
	Node->SetColor(MSTNodeColor);
	int32 ParentNodeIndex = Parents[NodeIndex];
	if (ParentNodeIndex == -1)
	{
		ensure(NodeIndex == Root);
		return;
	}
	UGraphEdge* Edge = Graph->GetEdge(ParentNodeIndex, NodeIndex);
	Edge->SetColor(MSTEdgeColor);
	ensure(!EMST.Contains(Edge));
	EMST.Add(Edge);
}

void UMSTPrimGraphAlgorithm::Relax(int32 u, int32 v)
{
	int32 vi = Q.Find(v);
	ensure(vi != -1);
	int32 CurrPriority = Q.GetPriority(vi);
	UGraphEdge* Edge = Graph->GetEdge(u, v);
	int32 w = Edge->GetWeight();
	if (w < CurrPriority)
	{
		Parents[v] = u;
		Q.DecreasePriority(v, w);
	}
}

