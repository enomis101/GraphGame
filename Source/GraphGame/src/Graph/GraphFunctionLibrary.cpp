#include "Graph/GraphFunctionLibrary.h"
#include "Graph/GameGraph.h"
#include <Containers/Queue.h>
#include "Graph/GameGraphNode.h"

bool UGraphFunctionLibrary::IsGraphConnected(UGraph* G)
{
	if (!ensure(G && G->Nodes.Num() > 0))
	{
		return false;
	}

	TQueue<int32> Q;
	TSet<int32> Visited;
	Q.Enqueue(0);
	Visited.Add(0);
	while (!Q.IsEmpty())
	{
		int32 Curr;
		ensure(Q.Dequeue(Curr));
		for (int32 x : G->Nodes[Curr]->Edges)
		{
			if (!Visited.Contains(x))
			{
				Visited.Add(x);
				Q.Enqueue(x);
			}
		}
	}

	return Visited.Num() == G->Nodes.Num();
}
