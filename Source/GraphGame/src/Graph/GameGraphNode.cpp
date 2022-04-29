// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GameGraphNode.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include "Graph/GameGraphEdge.h"
#include <Components/TextRenderComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Materials/MaterialInstanceDynamic.h>

// Sets default values
AGameGraphNode::AGameGraphNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereHitBox = CreateDefaultSubobject<USphereComponent>(FName("SphereHitBox"));
	RootComponent = SphereHitBox;

}

// Called when the game starts or when spawned
void AGameGraphNode::BeginPlay()
{
	Super::BeginPlay();
	if (SphereHitBox) 
	{
		//SphereHitBox->SetSphereRadius(SphereHitBoxRadius);
		SphereHitBox->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
		SphereHitBox->SetSphereRadius(SphereHitBoxRadius);
		FQuat QuatRot(FVector::UpVector, PI);
		SphereHitBox->SetWorldRotation(QuatRot);
	}

	UStaticMeshComponent* Mesh = FindComponentByClass<UStaticMeshComponent>();
	if (Mesh)
	{
		Material = Mesh->CreateDynamicMaterialInstance(0);
		if (Material)
		{
			Material->SetVectorParameterValue(MaterialParameterName, DefaultColor);
			CurrentColor = DefaultColor;
		}
	}
}

void AGameGraphNode::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ThisClass, SphereHitBoxRadius)) 
	{
		if (SphereHitBox)
		{
			SphereHitBox->SetSphereRadius(SphereHitBoxRadius);
		}
	}

}

// Called every frame

void AGameGraphNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AGameGraphNode::ContainsEdge(int32 OtherNode)
{
	for (int32 Node : Edges)
	{
		if (Node == OtherNode)
			return true;
	}
	return false;
}

void AGameGraphNode::AddEdge(int32 NewEdge)
{
	ensure(!ContainsEdge(NewEdge));
	Edges.Add(NewEdge);
}

void AGameGraphNode::SetNodeName(int32 i)
{
	Id = i;
	UTextRenderComponent* TextComponent = FindComponentByClass< UTextRenderComponent>();
	if (TextComponent)
	{
		TextComponent->SetText(FText::FromString(GetNodeName()));
	}
}

FString AGameGraphNode::GetNodeName()
{
	return "N" + FString::FromInt(Id);
}

void AGameGraphNode::Reset()
{
	SetDefaultColor();
	SetNodeName(Id);
}

void AGameGraphNode::SetColor(FLinearColor InColor)
{
	if (CurrentColor == InColor)
		return;

	if (Material)
	{
		Material->SetVectorParameterValue(MaterialParameterName, InColor);
		CurrentColor = InColor;
	}

}

void AGameGraphNode::SetAdditionalText(const FString& InString)
{
	UTextRenderComponent* TextComponent = FindComponentByClass< UTextRenderComponent>();
	if (TextComponent)
	{
		TextComponent->SetText(FText::FromString(GetNodeName() + "\n" + InString));
	}
}

void AGameGraphNode::SetDefaultColor()
{
	if (CurrentColor == DefaultColor)
		return;

	if (Material)
	{
		Material->SetVectorParameterValue(MaterialParameterName, DefaultColor);
		CurrentColor = DefaultColor;
	}
}

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
		SpawnLocation = InSpawnLocation;
	}
	

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
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

