#include "Graph/GameGraph.h"
#include <Kismet/KismetMathLibrary.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include "Graph/Algorithms/BFSGraphAlgorithm.h"
#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"
#include "Actors/GraphNodeMeshActor.h"

#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarShowGraphDebugInfo(
	TEXT("ShowGraphDebugInfo"),
	0,
	TEXT("Show Graph Debug Info"),
	ECVF_Cheat
);
#endif


/*
* 
* UGraph
* 
*/

//UGraph::UGraph(const FObjectInitializer& ObjectInitializer)
//	:Super(ObjectInitializer)
//{
//
//}


void UGraph::Init(bool bNewGraph, bool bIsDir)
{
	if (bNewGraph)
	{
		bIsDirected = bIsDir;
	}

	PC = Cast<AGameplayPlayerController>(GetOuter());
	CameraComponent = PC ? PC->GetCameraComponent() : nullptr;

	for (UGraphNode* Node : Nodes)
	{
		Node->Init();
	}

	EdgeMap.Empty();
	for (UGraphEdge* Edge : Edges)
	{
		Edge->Init();
		UGraphNode* FirstNode = Nodes[Edge->Node1];
		UGraphNode* SecondNode = Nodes[Edge->Node2];
		if (bIsDirected)
		{
			if (!EdgeMap.Contains(FirstNode))
			{
				EdgeMap.Add(FirstNode, TMap<UGraphNode*, UGraphEdge*>());

			}
			EdgeMap[FirstNode].Add(SecondNode, Edge);
			//If the edge is parallel to another one, offset it right
			if (SecondNode->ContainsEdge(Edge->Node1))
			{
				Edge->OffsetRight();
			}
		}
		else
		{
			if (!EdgeMap.Contains(FirstNode))
			{
				EdgeMap.Add(FirstNode, TMap<UGraphNode*, UGraphEdge*>());

			}
			EdgeMap[FirstNode].Add(SecondNode, Edge);
			if (!EdgeMap.Contains(SecondNode))
			{
				EdgeMap.Add(SecondNode, TMap<UGraphNode*, UGraphEdge*>());

			}
			EdgeMap[SecondNode].Add(FirstNode, Edge);
		}
	}
}

void UGraph::DeInit()
{

	for (UGraphNode* Node : Nodes)
	{
		Node->DeInit();
	}

	for (UGraphEdge* Edge : Edges)
	{
		Edge->DeInit();
	}

	EdgeMap.Empty();
}


void UGraph::SerializeGraph(FArchive& Ar)
{
	Ar << GraphName;
	Ar << bIsDirected;

	if (Ar.IsSaving())
	{
		int32 NodesNum = Nodes.Num();
		Ar << NodesNum;
		for (UGraphNode* Node : Nodes)
		{
			Node->SerializeNode(Ar);
		}

		int32 EdgesNum = Edges.Num();
		Ar << EdgesNum;
		for (UGraphEdge* Edge : Edges)
		{
			Edge->SerializeEdge(Ar, this);
		}
	}
	else
	{
		Nodes.Empty();
		Edges.Empty();
		EdgeMap.Empty();

		int32 NodesNum = 0;
		Ar << NodesNum;
		for (int32 i = 0; i < NodesNum; i++)
		{
			UGraphNode* NewNode = NewObject<UGraphNode>(this, NodeClass);
			Nodes.Add(NewNode);
			NewNode->SerializeNode(Ar);
		}

		int32 EdgesNum = 0;
		Ar << EdgesNum;
		for (int32 i = 0; i < EdgesNum; i++)
		{
			UGraphEdge* NewEdge = NewObject<UGraphEdge>(this, bIsDirected ? DirectedEdgeClass : UnDirectedEdgeClass);
			Edges.Add(NewEdge);
			NewEdge->SerializeEdge(Ar, this);
		}
	}

}



void UGraph::OnMouseClick()
{
	if (CurrentRunningAlgorithm)
	{
		return;
	}

	LastClickedNode = nullptr;

	if (!CameraComponent || !PC)
		return;
	FVector PointOnZeroPlane;
	GetPointOnZeroPlane(PointOnZeroPlane);

	FHitResult HitResult;
	if (IsValidSpawnPoint(PointOnZeroPlane, HitResult))
	{
		if (UGraphNode* NewNode = SpawnNewNode(PointOnZeroPlane))
		{
			Nodes.Add(NewNode);
		}
	}
	else
	{
		AGraphNodeMeshActor* NodeMeshActor = Cast<AGraphNodeMeshActor>(HitResult.Actor);
		if (NodeMeshActor)
		{
			ensure(NodeMeshActor->GraphNode);
			LastClickedNode = NodeMeshActor->GraphNode;
		}
	}
}

void UGraph::OnMouseReleased()
{
	if (CurrentRunningAlgorithm)
	{
		return;
	}

	FVector PointOnZeroPlane;
	if (!GetPointOnZeroPlane(PointOnZeroPlane))
		return;
	if (!LastClickedNode)
		return;

	FHitResult HitResult;
	if (IsValidSpawnPoint(PointOnZeroPlane, HitResult))
	{
		if (UGraphNode* NewNode = SpawnNewNode(PointOnZeroPlane))
		{
			Nodes.Add(NewNode);
			SpawnNewEdge(LastClickedNode, NewNode);
		}
	}
	else
	{
		AGraphNodeMeshActor* NodeMeshActor = Cast<AGraphNodeMeshActor>(HitResult.Actor);
		if (NodeMeshActor)
		{
			ensure(NodeMeshActor->GraphNode);
			UGraphNode* OtherNode = NodeMeshActor->GraphNode;
			if (OtherNode != LastClickedNode)
			{
				SpawnNewEdge(LastClickedNode, OtherNode);
			}
		}

	}

	//if (DebugDataAsset) {

//	FVector SphereCenter = PointOnZeroPlane;
//	//GEngine->AddOnScreenDebugMessage(-1, 5.f, bHitObject ? FColor::Red : FColor::Green,
//		//FString::Printf(TEXT("MOUSE CLICK on Screen Pos %f %f\n SpehereCenter: %s"), X, Y, *(SphereCenter.ToString())));
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, bHitObject ? FColor::Red : FColor::Green, FString::Printf(TEXT("MOUSE CLICK")));

//	DrawDebugSphere(GetWorld() /*InWorld*/, SphereCenter /*Center*/, DebugDataAsset->SphereRadius /*Radius*/, 16 /*Segments*/,
//		FColor::Green /*Color*/, false /*bPersistentLines*/, 5.f /*LifeTime*/, 0 /*DepthPriority*/, 0.f /*Thickness*/);
//}
}

void UGraph::ResetAppearance()
{
	for (UGraphNode* Node : Nodes)
	{
		Node->ResetAppearance();
	}

	for (UGraphEdge* Edge : Edges)
	{
		Edge->ResetAppearance();
	}
}

void UGraph::StartAlgorithm(UGraphAlgorithmParams* InParams)
{
	DestroyCurrentAlgorithm();
	ResetAppearance();
	ensure(InParams);
	if (AlgorithmClasses.Contains(InParams->AlgoType))
	{
		CurrentRunningAlgorithm = NewObject<UGraphAlgorithm>(this, AlgorithmClasses[InParams->AlgoType]);
		if (CurrentRunningAlgorithm)
		{
			CurrentRunningAlgorithm->Init(this);
			CurrentRunningAlgorithm->Start(InParams);

			InParams->ConditionalBeginDestroy();
		}
	}

}

void UGraph::StepAlgorithm()
{
	if (CurrentRunningAlgorithm)
	{
		if (CurrentRunningAlgorithm->Step())
		{
			CurrentRunningAlgorithm->End();
			DestroyCurrentAlgorithm();
		}
	}
}

void UGraph::RunAlgorithm()
{
	if (CurrentRunningAlgorithm)
	{
		while(true)
		{
			if (CurrentRunningAlgorithm->Step())
				break;
		}

		CurrentRunningAlgorithm->End();
		DestroyCurrentAlgorithm();
	}
}

void UGraph::DestroyCurrentAlgorithm()
{
	if (CurrentRunningAlgorithm)
	{
		CurrentRunningAlgorithm->ConditionalBeginDestroy();
		CurrentRunningAlgorithm = nullptr;
	}
}

UGraphEdge* UGraph::GetEdge(int32 Node1Index, int32 Node2Index)
{
	return GetEdge(Nodes[Node1Index], Nodes[Node2Index]);
}

UGraphEdge* UGraph::GetEdge(UGraphNode* Node1, UGraphNode* Node2)
{
	ensure(EdgeMap.Contains(Node1) && EdgeMap[Node1].Contains(Node2));
	return EdgeMap[Node1][Node2];
}

UGraphNode* UGraph::GetNodeAtIndex(int32 Index, bool bFallbackLastClicked /*= false*/)
{
	if (Nodes.IsValidIndex(Index))
	{
		return Nodes[Index];
	}
	else if (bFallbackLastClicked)
	{
		return LastClickedNode;
	}

	return nullptr;
}

UGraphNode* UGraph::GetNodeById(int32 NodeId)
{
	for (UGraphNode* Node : Nodes)
	{
		if (NodeId == Node->GetId())
			return Node;
	}
	return nullptr;
}

int32 UGraph::GetNodeIndexByValue(int32 Val)
{
	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		if (Nodes[i]->GetValue() == Val)
			return i;
	}
	return -1;
}

int32 UGraph::GetNodeIndex(UGraphNode* InNode)
{
	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		if (Nodes[i] == InNode)
			return i;
	}
	return -1;
}

bool UGraph::GetPointOnZeroPlane(FVector& OutVector)
{
	float X, Y;

	PC->GetMousePosition(X, Y);

	FVector2D ScreenPosition(X, Y);
	FVector WorldPosition;
	FVector WorldDirection;
	UGameplayStatics::DeprojectScreenToWorld(PC, ScreenPosition, WorldPosition, WorldDirection);
	FVector CameraLoc = CameraComponent->GetComponentLocation();

	float cosTheta = FVector::DotProduct(WorldDirection, -FVector::UpVector);
	float dist = cosTheta != 0.f ? (CameraLoc.Z / cosTheta) : CameraLoc.Z;
	OutVector = CameraLoc + dist * WorldDirection;
	OutVector.Z = 0;
	return true;
}

UGraphNode* UGraph::SpawnNewNode(const FVector& SpawnLocation)
{
	UGraphNode* NewNode = NewObject<UGraphNode>(this, NodeClass);
	if (NewNode)
	{
		NewNode->Init(Nodes.Num(), SpawnLocation);
	}
	return NewNode;
}

void UGraph::SpawnNewEdge(UGraphNode* Node1, UGraphNode* Node2)
{
	if (bIsDirected)
	{
		SpawnDirectedEdge(Node1, Node2);
	}
	else
	{
		SpawnUnDirectedEdge(Node1, Node2);
	}
}

void UGraph::SpawnDirectedEdge(UGraphNode* Node1, UGraphNode* Node2)
{
	int32 Node1Index = GetNodeIndex(Node1);
	int32 Node2Index = GetNodeIndex(Node2);
	if (Node1->ContainsEdge(Node2Index))
	{
		return;
	}

	UGraphEdge* Edge = NewObject<UGraphEdge>(this, DirectedEdgeClass);

	if (Edge)
	{
		Edge->Init(Node1Index, Node2Index, Edges.Num());
		Edges.Add(Edge);
		Node1->AddEdge(Node2Index);
		if (!EdgeMap.Contains(Node1))
		{
			EdgeMap.Add(Node1, TMap<UGraphNode*, UGraphEdge*>());

		}
		EdgeMap[Node1].Add(Node2, Edge);
	}

	if (Node2->ContainsEdge(Node1Index))
	{
		ensure(EdgeMap.Contains(Node2));
		UGraphEdge* OtherEdge = EdgeMap[Node2][Node1];
		ensure(OtherEdge);
		Edge->OffsetRight();
		OtherEdge->OffsetRight();
	}
}

void UGraph::SpawnUnDirectedEdge(UGraphNode* Node1, UGraphNode* Node2)
{
	int32 Node1Index = GetNodeIndex(Node1);
	int32 Node2Index = GetNodeIndex(Node2);
	if (Node1->ContainsEdge(Node2Index))
	{
		ensure(Node2->ContainsEdge(Node1Index));
		return;
	}

	UGraphEdge* Edge = NewObject<UGraphEdge>(this, UnDirectedEdgeClass);

	if (Edge)
	{
		Edge->Init(Node1Index, Node2Index, Edges.Num());
		Edges.Add(Edge);

		Node1->AddEdge(Node2Index);
		Node2->AddEdge(Node1Index);
		if (!EdgeMap.Contains(Node1))
		{
			EdgeMap.Add(Node1, TMap<UGraphNode*, UGraphEdge*>());

		}
		EdgeMap[Node1].Add(Node2, Edge);
		if (!EdgeMap.Contains(Node2))
		{
			EdgeMap.Add(Node2, TMap<UGraphNode*, UGraphEdge*>());

		}
		EdgeMap[Node2].Add(Node1, Edge);
	}

}



bool UGraph::IsValidSpawnPoint(const FVector& SpawnLocation, FHitResult& OutHitResult)
{
	//Trace a ray vertically
	FVector CameraLoc = CameraComponent->GetComponentLocation();
	FVector StartPosition = SpawnLocation;
	StartPosition.Z = CameraLoc.Z - 10.f;
	FVector EndPosition = SpawnLocation - 10.f * FVector::UpVector;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, StartPosition, EndPosition, ECC_WorldDynamic);
	if (Cast<UGraphNode>(OutHitResult.Actor) != nullptr)
	{
		return false;
	}
	//Spawn a test actor to check for collision
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	AActor* TestNode = GetWorld()->SpawnActor<AActor>(TestNodeClass, SpawnTransform, SpawnParameters);
	if (!TestNode)
	{
		return false;
	}
	TestNode->Destroy();
	return true;
}

void UGraph::PrintGraph()
{
	FString str;
	for (UGraphNode* Node : Nodes)
	{
		ensure(Node);
		str += Node->GetNodeName() + ": ";
		for (int32 OtherNodeIndex : Node->Edges)
		{
			UGraphNode* OtherNode = Nodes[OtherNodeIndex];
			UGraphEdge* Edge = EdgeMap[Node][OtherNode];
			str += "[" + OtherNode->GetNodeName() + ", " + Edge->GetEdgeName() + "]";
		}
		str += "\n";
	}
	GEngine->AddOnScreenDebugMessage(111, 5.f, FColor::Green, str);
}

//void UGraph::BeginDestroy()
//{
//	Super::BeginDestroy();
//
//
//}

