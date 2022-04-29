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

AGameGraph::AGameGraph()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameGraph::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#if !UE_BUILD_SHIPPING
	if (CVarShowGraphDebugInfo.GetValueOnGameThread() == 1)
	{
		PrintGraph();
	}
#endif
}

void AGameGraph::OnMouseClick()
{
	LastClickedNode = nullptr;

	if (!CameraComponent || !PC)
		return;
	FVector PointOnZeroPlane;
	GetPointOnZeroPlane(PointOnZeroPlane);

	FHitResult HitResult;
	if (!CheckValidSpawnPoint(PointOnZeroPlane, HitResult))
	{
		if (AGameGraphNode* NewNode = SpawnNewNode(PointOnZeroPlane))
		{
			Nodes.Add(NewNode);
		}
	}
	else 
	{
		LastClickedNode = Cast<AGameGraphNode>(HitResult.Actor);
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

void AGameGraph::OnMouseReleased()
{
	FVector PointOnZeroPlane;
	if (!GetPointOnZeroPlane(PointOnZeroPlane))
		return;
	if (!LastClickedNode)
		return;

	FHitResult HitResult;
	if (!CheckValidSpawnPoint(PointOnZeroPlane, HitResult))
	{
		if (AGameGraphNode* NewNode = SpawnNewNode(PointOnZeroPlane))
		{
			Nodes.Add(NewNode);
			SpawnNewEdge(LastClickedNode, NewNode);
		}
	}
	else
	{
		AGameGraphNode* OtherNode = Cast<AGameGraphNode>(HitResult.Actor);
		if(OtherNode != LastClickedNode)
		{
			SpawnNewEdge(LastClickedNode, OtherNode);
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

void AGameGraph::Reset()
{
	for (AGameGraphNode* Node : Nodes)
	{
		Node->Reset();
	}

	for (AGameGraphEdge* Edge : EdgeList)
	{
		Edge->Reset();
	}
}

void AGameGraph::StartAlgorithm(UGraphAlgorithmParams* InParams)
{
	DestroyCurrentAlgorithm();
	Reset();
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

void AGameGraph::StepAlgorithm()
{
	ensure(CurrentRunningAlgorithm);
	if (CurrentRunningAlgorithm)
	{
		if (CurrentRunningAlgorithm->Step())
		{
			CurrentRunningAlgorithm->End();
			DestroyCurrentAlgorithm();
		}
	}
}

void AGameGraph::DestroyCurrentAlgorithm()
{
	if (CurrentRunningAlgorithm)
	{
		CurrentRunningAlgorithm->ConditionalBeginDestroy();
		CurrentRunningAlgorithm = nullptr;
	}
}

AGameGraphEdge* AGameGraph::GetEdge(int32 Node1Index, int32 Node2Index)
{
	return GetEdge(Nodes[Node1Index], Nodes[Node2Index]);
}

AGameGraphEdge* AGameGraph::GetEdge(AGameGraphNode* Node1, AGameGraphNode* Node2)
{
	ensure(Edges.Contains(Node1) && Edges[Node1].Contains(Node2));
	return Edges[Node1][Node2];
}

AGameGraphNode* AGameGraph::GetNodeAtIndex(int32 Index, bool bFallbackLastClicked /*= false*/)
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

int32 AGameGraph::GetNodeIndexByValue(int32 Val)
{
	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		if (Nodes[i]->GetValue() == Val)
			return i;
	}
	return -1;
}

int32 AGameGraph::GetNodeIndex(AGameGraphNode* InNode)
{
	for (int32 i = 0; i < Nodes.Num(); i++)
	{
		if (Nodes[i] == InNode)
			return i;
	}
	return -1;
}

void AGameGraph::SetDirected(bool bInValue)
{
	bIsDirected = bInValue;
}

bool AGameGraph::GetPointOnZeroPlane(FVector& OutVector)
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

AGameGraphNode* AGameGraph::SpawnNewNode(const FVector& SpawnLocation)
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	AGameGraphNode* NewNode = GetWorld()->SpawnActor<AGameGraphNode>(NodeClass, SpawnTransform, SpawnParameters);
	if (NewNode)
	{
		NewNode->SetNodeName(nNodes);
		nNodes++;
	}
	return NewNode;
}

void AGameGraph::SpawnNewEdge(AGameGraphNode* Node1, AGameGraphNode* Node2)
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

void AGameGraph::SpawnDirectedEdge(AGameGraphNode* Node1, AGameGraphNode* Node2)
{
	int32 Node1Index = GetNodeIndex(Node1);
	int32 Node2Index = GetNodeIndex(Node2);
	if (Node1->ContainsEdge(Node2Index))
	{
		return;
	}

	AGameGraphEdge* Edge = SpawnAndPlaceEdge(Node1, Node2, true /*bDirectedEdge*/);

	if (Edge)
	{
		Edge->Init(Node1, Node2, nEdges);
		nEdges++;
		EdgeList.Add(Edge);
		Node1->AddEdge(Node2Index);
		if (!Edges.Contains(Node1))
		{
			Edges.Add(Node1, TMap<AGameGraphNode*, AGameGraphEdge*>());

		}
		Edges[Node1].Add(Node2, Edge);
	}

	if (Node2->ContainsEdge(Node1Index))
	{
		ensure(Edges.Contains(Node2));
		AGameGraphEdge* OtherEdge = Edges[Node2][Node1];
		ensure(OtherEdge);
		Edge->OffsetRight();
		OtherEdge->OffsetRight();
	}
}

void AGameGraph::SpawnUnDirectedEdge(AGameGraphNode* Node1, AGameGraphNode* Node2)
{
	int32 Node1Index = GetNodeIndex(Node1);
	int32 Node2Index = GetNodeIndex(Node2);
	if (Node1->ContainsEdge(Node2Index))
	{
		ensure(Node2->ContainsEdge(Node1Index));
		return;
	}

	AGameGraphEdge* Edge = SpawnAndPlaceEdge(Node1, Node2, false /*bDirectedEdge*/);

	if (Edge)
	{
		Edge->Init(Node1, Node2, nEdges);
		nEdges++;
		EdgeList.Add(Edge);

		Node1->AddEdge(Node2Index);
		Node2->AddEdge(Node1Index);
		if (!Edges.Contains(Node1))
		{
			Edges.Add(Node1, TMap<AGameGraphNode*, AGameGraphEdge*>());

		}
		Edges[Node1].Add(Node2, Edge);
		if (!Edges.Contains(Node2))
		{
			Edges.Add(Node2, TMap<AGameGraphNode*, AGameGraphEdge*>());

		}
		Edges[Node2].Add(Node1, Edge);
	}

}

AGameGraphEdge* AGameGraph::SpawnAndPlaceEdge(AGameGraphNode* Node1, AGameGraphNode* Node2, bool bDirectedEdge /*= true*/)
{
	FVector Node1Location = Node1->GetActorLocation();
	FVector Node2Location = Node2->GetActorLocation();
	FVector EdgeDirection = Node2Location - Node1Location;

	float EdgeLength = EdgeDirection.Size() - Node1->SphereHitBoxRadius * 2.f;
	EdgeDirection.Normalize();

	FVector SpawnLocation = Node1Location + EdgeDirection * Node1->SphereHitBoxRadius;
	FVector RightVector = FVector::CrossProduct(EdgeDirection, FVector::UpVector);
	//FRotator SpawnRotation = UKismetMathLibrary::MakeRotationFromAxes(EdgeDirection /*Forward*/, RightVector /*Right*/, FVector::UpVector /*Up*/);
	//FVector SpawnScale(1.f, 1.f, EdgeLength);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Node1Location, Node2Location);
	//FVector SpawnScale(1.f);
	FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.f));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return GetWorld()->SpawnActor<AGameGraphEdge>(bDirectedEdge ? DirectedEdgeClass : UnDirectedEdgeClass, SpawnTransform, SpawnParameters);
}

bool AGameGraph::CheckValidSpawnPoint(const FVector& SpawnLocation, FHitResult& OutHitResult)
{
	//Trace a ray vertically
	FVector CameraLoc = CameraComponent->GetComponentLocation();
	FVector StartPosition = SpawnLocation;
	StartPosition.Z = CameraLoc.Z - 10.f;
	FVector EndPosition = SpawnLocation - 10.f * FVector::UpVector;

	GetWorld()->LineTraceSingleByChannel(OutHitResult, StartPosition, EndPosition, ECC_WorldDynamic);
	return Cast<UGraphNode>(OutHitResult.Actor) != nullptr;
}

void AGameGraph::PrintGraph()
{
	FString str;
	for (AGameGraphNode* Node : Nodes)
	{
		ensure(Node);
		str += Node->GetNodeName() + ": ";
		for (int32 OtherNodeIndex : Node->Edges)
		{
			AGameGraphNode* OtherNode = Nodes[OtherNodeIndex];
			AGameGraphEdge* Edge = Edges[Node][OtherNode];
			str += "[" + OtherNode->GetNodeName() + ", " + Edge->GetEdgeName() + "]";
		}
		str += "\n";
	}
	GEngine->AddOnScreenDebugMessage(111, 5.f, FColor::Green, str);
}

void AGameGraph::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AGameplayPlayerController>(GetOwner());
	CameraComponent = PC ? PC->GetCameraComponent() : nullptr;
}



/*
* 
* 
* UGraph
* 
* 
* 
*/




void UGraph::Init()
{

	PC = Cast<AGameplayPlayerController>(GetOuter());
	CameraComponent = PC ? PC->GetCameraComponent() : nullptr;
}

void UGraph::OnMouseClick()
{
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

	for (UGraphEdge* Edge : EdgeList)
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
			//CurrentRunningAlgorithm->Init(this);
			CurrentRunningAlgorithm->Start(InParams);

			InParams->ConditionalBeginDestroy();
		}
	}

}

void UGraph::StepAlgorithm()
{
	ensure(CurrentRunningAlgorithm);
	if (CurrentRunningAlgorithm)
	{
		if (CurrentRunningAlgorithm->Step())
		{
			CurrentRunningAlgorithm->End();
			DestroyCurrentAlgorithm();
		}
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
	ensure(Edges.Contains(Node1) && Edges[Node1].Contains(Node2));
	return Edges[Node1][Node2];
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

void UGraph::SetDirected(bool bInValue)
{
	bIsDirected = bInValue;
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
		NewNode->Init(nNodes, SpawnLocation);
		nNodes++;
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
		Edge->Init(Node1, Node2, nEdges);
		nEdges++;
		EdgeList.Add(Edge);
		Node1->AddEdge(Node2Index);
		if (!Edges.Contains(Node1))
		{
			Edges.Add(Node1, TMap<UGraphNode*, UGraphEdge*>());

		}
		Edges[Node1].Add(Node2, Edge);
	}

	if (Node2->ContainsEdge(Node1Index))
	{
		ensure(Edges.Contains(Node2));
		UGraphEdge* OtherEdge = Edges[Node2][Node1];
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
		Edge->Init(Node1, Node2, nEdges);
		nEdges++;
		EdgeList.Add(Edge);

		Node1->AddEdge(Node2Index);
		Node2->AddEdge(Node1Index);
		if (!Edges.Contains(Node1))
		{
			Edges.Add(Node1, TMap<UGraphNode*, UGraphEdge*>());

		}
		Edges[Node1].Add(Node2, Edge);
		if (!Edges.Contains(Node2))
		{
			Edges.Add(Node2, TMap<UGraphNode*, UGraphEdge*>());

		}
		Edges[Node2].Add(Node1, Edge);
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
	if (Cast<AGameGraphNode>(OutHitResult.Actor) != nullptr)
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
			UGraphEdge* Edge = Edges[Node][OtherNode];
			str += "[" + OtherNode->GetNodeName() + ", " + Edge->GetEdgeName() + "]";
		}
		str += "\n";
	}
	GEngine->AddOnScreenDebugMessage(111, 5.f, FColor::Green, str);
}


