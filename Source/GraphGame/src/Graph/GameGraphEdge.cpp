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


void UGraphEdge::Init(UGraphNode* N1, UGraphNode* N2, int32 i)
{
	if (N1 && N2 && i >= 0)
	{
		FirstNode = N1;
		SecondNode = N2;
		Id = i;
	}


	MeshActor = SpawnAndPlaceMeshActor();
	if (MeshActor)
	{
		InitialWorldLocation = MeshActor->GetActorLocation();

		InitMeshes();

		ResetAppearance();
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

void UGraphEdge::InitMeshes()
{
	ensure(MeshActor);
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
	}

	if (HeadMesh)
	{
		Dir.Normalize();
		FVector HeadLocation = SecondNode->GetLocation() - SecondNode->Radius * Dir;
		HeadMesh->SetWorldLocation(HeadLocation);
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
	}
}

AGraphEdgeMeshActor* UGraphEdge::SpawnAndPlaceMeshActor()
{
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

void UGraphEdge::SetDefaultName()
{
	if (Text)
	{
		Text->SetText(FText::FromString(GetEdgeName()));
	}
}

void UGraphEdge::OffsetRight()
{
	if (MeshActor)
	{
		FVector RightVector = MeshActor->GetActorTransform().TransformVector(FVector::RightVector);
		MeshActor->SetActorLocation(InitialWorldLocation + RightVector * RightOffsetAmount);
	}
}

void UGraphEdge::ResetOffset()
{
	if (MeshActor)
	{
		MeshActor->SetActorLocation(InitialWorldLocation);
	}
}

FString UGraphEdge::GetEdgeName()
{
	return "E" + FString::FromInt(Id);
}

void UGraphEdge::SetSelected()
{
	if (Material)
	{
		Material->SetScalarParameterValue(SelParameterName, 1.f);
	}
}

void UGraphEdge::SerializeEdge(FArchive& Ar, UGraph* OuterGraph)
{
	Ar << Id;
	if (Ar.IsSaving())
	{
		int32 FirstNodeId = FirstNode->GetId();
		int32 SecondNodeId = SecondNode->GetId();
		Ar << FirstNodeId;
		Ar << SecondNodeId;
	}
	else
	{
		int32 FirstNodeId, SecondNodeId;
		Ar << FirstNodeId;
		Ar << SecondNodeId;
		FirstNode = OuterGraph->GetNodeById(FirstNodeId);
		SecondNode = OuterGraph->GetNodeById(SecondNodeId);
	}
}

void UGraphEdge::ResetAppearance()
{
	SetDefaultName();

	if (Material)
	{
		Material->SetScalarParameterValue(SelParameterName, 0.f);
	}
}

