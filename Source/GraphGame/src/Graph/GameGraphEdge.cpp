// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GameGraphEdge.h"
#include "Graph/GameGraphNode.h"
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <Materials/MaterialInstance.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Components/TextRenderComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include "Graph/GameGraph.h"
#include <Serialization/Archive.h>

#pragma optimize("",off)



/*
* 
* 
* UGRAPHEDGE
* 
* 
* 
*/


void UGraphEdge::Init(int32 N1, int32 N2, int32 i)
{
	if (N1 >= 0 && N2 >= 0 && i >= 0)
	{
		Node1 = N1;
		Node2 = N2;
		Id = i;
	}


	MeshActor = SpawnAndPlaceMeshActor();
	if (MeshActor)
	{
		InitialWorldLocation = MeshActor->GetActorLocation();

		InitMeshes();

		ResetAppearance();
		MeshActor->SetActorEnableCollision(false);
	}
}

void UGraphEdge::DeInit()
{
	if (MeshActor)
	{
		MeshActor->GraphEdge = nullptr;
		ensure(MeshActor->Destroy());
		MeshActor = nullptr;
		Material = nullptr;
		Text = nullptr;
	}
}

void UGraphEdge::UpdatePosition(const FVector& NewPosition)
{
	if (MeshActor)
	{
		UpdateMeshActorPosition();

		InitMeshes();

		ResetAppearance();
		MeshActor->SetActorEnableCollision(false);
	}
}

void UGraphEdge::InitMeshes()
{
	UGraph* Graph = Cast<UGraph>(GetOuter());
	ensure(MeshActor && Graph);

	UGraphNode* FirstNode = Graph->Nodes[Node1];
	UGraphNode* SecondNode = Graph->Nodes[Node2];

	FVector Dir = SecondNode->GetLocation() - FirstNode->GetLocation();
	Length = Dir.Size() - FirstNode->Radius * 2.f;
	MeshActor->GraphEdge = this;

	UStaticMeshComponent* ShaftMesh = nullptr;
	UStaticMeshComponent* HeadMesh = nullptr;

	TArray<UActorComponent*> Components = MeshActor->GetComponentsByClass(UStaticMeshComponent::StaticClass());
	for (UActorComponent* Component : Components)
	{
		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(Component);
		if (MeshComponent->ComponentHasTag("ArrowShaft"))
		{
			ShaftMesh = MeshComponent;
		}
		else if (MeshComponent->ComponentHasTag("ArrowHead"))
		{
			HeadMesh = MeshComponent;
		}
		else
		{
			ensure(false);
		}
	}

	Material = nullptr;
	if (ShaftMesh)
	{
		ShaftMesh->SetRelativeScale3D(FVector(1.f, 1.f, Length / ZScale));
		Material = ShaftMesh->CreateDynamicMaterialInstance(0);
		ShaftMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (HeadMesh)
	{
		Dir.Normalize();
		FVector HeadLocation = SecondNode->GetLocation() - SecondNode->Radius * Dir;
		HeadMesh->SetWorldLocation(HeadLocation);
		HeadMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	Text = MeshActor->FindComponentByClass<UTextRenderComponent>();
	if (Text)
	{
		Text->SetWorldScale3D(FVector(1.f));
		FVector NormDir = Dir;

		float HalfLength = Length / 2.f + FirstNode->Radius;
		NormDir.Normalize();
		FVector NewLocation = FirstNode->GetLocation() + HalfLength * NormDir;
		Text->SetWorldLocation(NewLocation);
		Text->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

AGraphEdgeMeshActor* UGraphEdge::SpawnAndPlaceMeshActor()
{
	UGraph* Graph = Cast<UGraph>(GetOuter());
	ensure(Graph);

	UGraphNode* FirstNode = Graph->Nodes[Node1];
	UGraphNode* SecondNode = Graph->Nodes[Node2];

	FVector Node1Location = FirstNode->GetLocation();
	FVector Node2Location = SecondNode->GetLocation();
	FVector EdgeDirection = Node2Location - Node1Location;

	float EdgeLength = EdgeDirection.Size() - FirstNode->Radius * 2.f;
	EdgeDirection.Normalize();

	FVector SpawnLocation = Node1Location + EdgeDirection * FirstNode->Radius;
	FVector RightVector = FVector::CrossProduct(EdgeDirection, FVector::UpVector);
	//FRotator SpawnRotation = UKismetMathLibrary::MakeRotationFromAxes(EdgeDirection /*Forward*/, RightVector /*Right*/, FVector::UpVector /*Up*/);
	//FVector SpawnScale(1.f, 1.f, EdgeLength);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Node1Location, Node2Location);
	//FVector SpawnScale(1.f);
	FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.f));

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return GetWorld()->SpawnActor<AGraphEdgeMeshActor>(MeshClass, SpawnTransform, SpawnParameters);
}

void UGraphEdge::UpdateMeshActorPosition()
{
	UGraph* Graph = Cast<UGraph>(GetOuter());
	ensure(Graph);

	UGraphNode* FirstNode = Graph->Nodes[Node1];
	UGraphNode* SecondNode = Graph->Nodes[Node2];

	FVector Node1Location = FirstNode->GetLocation();
	FVector Node2Location = SecondNode->GetLocation();
	FVector EdgeDirection = Node2Location - Node1Location;

	float EdgeLength = EdgeDirection.Size() - FirstNode->Radius * 2.f;
	EdgeDirection.Normalize();

	FVector SpawnLocation = Node1Location + EdgeDirection * FirstNode->Radius;
	FVector RightVector = FVector::CrossProduct(EdgeDirection, FVector::UpVector);
	//FRotator SpawnRotation = UKismetMathLibrary::MakeRotationFromAxes(EdgeDirection /*Forward*/, RightVector /*Right*/, FVector::UpVector /*Up*/);
	//FVector SpawnScale(1.f, 1.f, EdgeLength);
	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(Node1Location, Node2Location);
	//FVector SpawnScale(1.f);
	FTransform SpawnTransform(SpawnRotation, SpawnLocation, FVector(1.f));

	MeshActor->SetActorTransform(SpawnTransform);
}

void UGraphEdge::SetDefaultName()
{
	if (Text)
	{
		Text->SetText(FText::FromString(GetEdgeName()));
	}
}

void UGraphEdge::OffsetRight()
{
	//if (MeshActor)
	//{
	//	FVector RightVector = MeshActor->GetActorTransform().TransformVector(FVector::RightVector);
	//	MeshActor->SetActorLocation(InitialWorldLocation + RightVector * RightOffsetAmount);
	//}
}

void UGraphEdge::ResetOffset()
{
	//if (MeshActor)
	//{
	//	MeshActor->SetActorLocation(InitialWorldLocation);
	//}
}

FString UGraphEdge::GetEdgeName()
{
	return "E" + FString::FromInt(Id);
}

void UGraphEdge::SetSelected()
{
	SetColor(SelectedColor);
}

void UGraphEdge::SetColor(FLinearColor InColor)
{
	if (CurrentColor == InColor)
		return;

	if (Material)
	{
		Material->SetVectorParameterValue(MaterialParameterName, InColor);
		CurrentColor = InColor;
	}

}

void UGraphEdge::SerializeEdge(FArchive& Ar, UGraph* OuterGraph)
{
	Ar << Id;
	if (Ar.IsSaving())
	{
		Ar << Node1;
		Ar << Node2;
	}
	else
	{
		Ar << Node1;
		Ar << Node2;
	}
}

void UGraphEdge::ResetAppearance()
{
	SetDefaultName();
	SetColor(DefaultColor);
}

