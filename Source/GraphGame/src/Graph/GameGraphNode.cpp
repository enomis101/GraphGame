// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GameGraphNode.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include "Graph/GameGraphEdge.h"
#include <Components/TextRenderComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>


/*
* 
* 
* 
* UGRAPHNODE
* 
* 
* 
*/



void UGraphNode::Init(int32 InId, FVector InSpawnLocation)
{
	if (InId >= 0)
	{
		Id = InId;
		CurrentLocation = InSpawnLocation;
	}
	

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(CurrentLocation);
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
	MeshActor = GetWorld()->SpawnActor<AGraphNodeMeshActor>(MeshClass, SpawnTransform, SpawnParameters);

	if (MeshActor)
	{
		InitMesh();
	}

	SetDefaultName();
	SetDefaultColor();
}

void UGraphNode::InitMesh()
{
	ensure(MeshActor);
	FQuat QuatRot(FVector::UpVector, PI);
	MeshActor->SetActorRotation(QuatRot);
	MeshActor->GraphNode = this;

	USphereComponent* SphereHitBox = MeshActor->FindComponentByClass<USphereComponent>();
	if (SphereHitBox)
	{
		//SphereHitBox->SetSphereRadius(SphereHitBoxRadius);
		SphereHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		SphereHitBox->SetSphereRadius(Radius);
	}

	Material = nullptr;
	UStaticMeshComponent* Mesh = MeshActor->FindComponentByClass<UStaticMeshComponent>();
	if (Mesh)
	{
		Material = Mesh->CreateDynamicMaterialInstance(0);
		if (Material)
		{
			Material->SetVectorParameterValue(MaterialParameterName, DefaultColor);
			CurrentColor = DefaultColor;
		}
	}

	Text = MeshActor->FindComponentByClass<UTextRenderComponent>();
}

bool UGraphNode::ContainsEdge(int32 OtherNode)
{
	for (int32 Node : Edges)
	{
		if (Node == OtherNode)
			return true;
	}
	return false;
}

void UGraphNode::AddEdge(int32 NewEdge)
{
	ensure(!ContainsEdge(NewEdge));
	Edges.Add(NewEdge);
}

void UGraphNode::SetDefaultName()
{
	if (Text)
	{
		Text->SetText(FText::FromString(GetNodeName()));
	}
}

FString UGraphNode::GetNodeName()
{
	return "N" + FString::FromInt(Id);
}

void UGraphNode::MoveNode(const FVector& NewLocation)
{
	CurrentLocation = NewLocation;
	ensure(MeshActor->TeleportTo(NewLocation, MeshActor->GetActorRotation()));
}

void UGraphNode::DeInit()
{
	if (MeshActor)
	{
		MeshActor->GraphNode = nullptr;
		ensure(MeshActor->Destroy());
		MeshActor = nullptr;
		Material = nullptr;
		Text = nullptr;
	}
}

void UGraphNode::SerializeNode(FArchive& Ar)
{
	Ar << CurrentLocation;
	Ar << Id;
	Ar << Edges;
}

void UGraphNode::ResetAppearance()
{
	SetDefaultColor();
	SetDefaultName();
}

void UGraphNode::SetColor(FLinearColor InColor)
{
	if (CurrentColor == InColor)
		return;

	if (Material)
	{
		Material->SetVectorParameterValue(MaterialParameterName, InColor);
		CurrentColor = InColor;
	}

}

void UGraphNode::SetAdditionalText(const FString& InString)
{
	if (Text)
	{
		Text->SetText(FText::FromString(GetNodeName() + "\n" + InString));
	}
}

void UGraphNode::SetDefaultColor()
{
	if (CurrentColor == DefaultColor)
		return;

	if (Material)
	{
		Material->SetVectorParameterValue(MaterialParameterName, DefaultColor);
		CurrentColor = DefaultColor;
	}
}

