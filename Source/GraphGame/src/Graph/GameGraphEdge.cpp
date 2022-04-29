// Fill out your copyright notice in the Description page of Project Settings.


#include "Graph/GameGraphEdge.h"
#include "Graph/GameGraphNode.h"
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <Materials/MaterialInstance.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Components/TextRenderComponent.h>
#include <Kismet/KismetMathLibrary.h>

#pragma optimize("",off)

// Sets default values
AGameGraphEdge::AGameGraphEdge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
}

// Called when the game starts or when spawned
void AGameGraphEdge::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGameGraphEdge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGameGraphEdge::SetEdgeName()
{
	TextComponent = FindComponentByClass< UTextRenderComponent>();
	if (!TextComponent)
	{
		TextComponent = NewObject< UTextRenderComponent>(this, UTextRenderComponent::StaticClass());
		TextComponent->SetupAttachment(RootComponent);
		FQuat Rot1(FVector::UpVector, PI / 2.f);
		FQuat Rot2(FVector::RightVector, PI / 2.f);
		TextComponent->SetRelativeRotation(Rot1 * Rot2);
	}
	if (TextComponent)
	{	
		TextComponent->SetWorldScale3D(FVector(1.f));
		FVector Dir = SecondNode->GetActorLocation() - FirstNode->GetActorLocation();

		float HalfLength = Length / 2.f + FirstNode->SphereHitBoxRadius;
		Dir.Normalize();
		FVector NewLocation = FirstNode->GetActorLocation() + HalfLength * Dir;
		TextComponent->SetWorldLocation(NewLocation);
		TextComponent->SetText(FText::FromString(GetEdgeName()));
		
	}
}



void AGameGraphEdge::InitMeshes()
{
	FVector Dir = SecondNode->GetActorLocation() - FirstNode->GetActorLocation();
	Length = Dir.Size() - FirstNode->SphereHitBoxRadius * 2.f;
	TArray<UActorComponent*> Components = GetComponentsByClass(UStaticMeshComponent::StaticClass());
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

	if (ShaftMesh)
	{
		ShaftMesh->SetRelativeScale3D(FVector(1.f, 1.f, Length / ZScale));
		Material = ShaftMesh->CreateDynamicMaterialInstance(0);
	}

	if (HeadMesh)
	{
		Dir.Normalize();
		FVector HeadLocation = SecondNode->GetActorLocation() - SecondNode->SphereHitBoxRadius * Dir;
		HeadMesh->SetWorldLocation(HeadLocation);
	}
}

void AGameGraphEdge::Init(AGameGraphNode* N1, AGameGraphNode* N2, int32 i)
{
	FirstNode = N1;
	SecondNode = N2;

	InitialWorldLocation = GetActorLocation();

	Id = i;

	InitMeshes();
	SetEdgeName();
}

void AGameGraphEdge::OffsetRight()
{
	FVector RightVector = GetActorTransform().TransformVector(FVector::RightVector);
	SetActorLocation(InitialWorldLocation + RightVector * RightOffsetAmount);
}

void AGameGraphEdge::ResetOffset()
{
	SetActorLocation(InitialWorldLocation);
}

FString AGameGraphEdge::GetEdgeName()
{
	return "E" + FString::FromInt(Id);
}

void AGameGraphEdge::SetSelected()
{
	if (Material)
	{
		Material->SetScalarParameterValue(SelParameterName, 1.f);
	}
}

void AGameGraphEdge::Reset()
{
	if (Material)
	{
		Material->SetScalarParameterValue(SelParameterName, 0.f);
	}
}


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
	FirstNode = N1;
	SecondNode = N2;

	Id = i;
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

void UGraphEdge::ResetAppearance()
{
	SetDefaultName();

	if (Material)
	{
		Material->SetScalarParameterValue(SelParameterName, 0.f);
	}
}

