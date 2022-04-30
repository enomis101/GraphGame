// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/GraphEdgeMeshActor.h"
#include "GameGraphEdge.generated.h"

struct FPropertyChangedEvent;
class UGraphNode;
class UStaticMeshComponent;
class UTextRenderComponent;
class UGraphNode;
class FArchive;
class UGraph;

UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphEdge : public UObject
{
	GENERATED_BODY()

public:

	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(SaveGame)
	UGraphNode* FirstNode = nullptr;

	UPROPERTY(SaveGame)
	UGraphNode* SecondNode = nullptr;

	void Init(UGraphNode* N1 = nullptr, UGraphNode* N2 = nullptr, int32 InId = -1);
	void DeInit();

	float GetWeight() { return Length; }

	void SerializeEdge(FArchive& Ar, UGraph* OuterGraph);

	//APPEARANCE
	void ResetAppearance();

	//Offset
	void OffsetRight();
	void ResetOffset();

	//Text
	FString GetEdgeName();
	void SetDefaultName();

	//Color
	void SetSelected();
	
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	//APPEARANCE
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ZScale = 20.f;

	UPROPERTY(EditAnywhere, Category = Appearance)
		float RightOffsetAmount = 20.f;

	UPROPERTY(EditAnywhere, Category = Appearance)
		FName SelParameterName = "Selected";

	UPROPERTY(EditAnywhere, Category = Appearance)
		TSubclassOf<AGraphEdgeMeshActor> MeshClass;

	//Save Game
	UPROPERTY(SaveGame)
		int32 Id;
private:
	
	void InitMeshes();
	AGraphEdgeMeshActor* SpawnAndPlaceMeshActor();

	UPROPERTY(Transient)
	AGraphEdgeMeshActor* MeshActor;

	UPROPERTY(Transient)
		UTextRenderComponent* Text;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* Material;

	FVector InitialWorldLocation;
	float Length;


	
};