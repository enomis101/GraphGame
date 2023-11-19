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

	int32 Node1;
	int32 Node2;

	void Init(int32 N1 = -1, int32 N2 = -1, int32 InId = -1);
	void DeInit();

	void UpdatePosition(const FVector& NewPosition);

	float GetWeight() const { return Length; }

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
	void SetColor(FLinearColor InColor);
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	//APPEARANCE
	UPROPERTY(EditAnywhere, Category = Appearance)
		float ZScale = 20.f;

	UPROPERTY(EditAnywhere, Category = Appearance)
		float RightOffsetAmount = 20.f;

	UPROPERTY(EditAnywhere, Category = Appearance)
	FLinearColor SelectedColor;

	UPROPERTY(EditAnywhere, Category = Appearance)
	FLinearColor DefaultColor;
		
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		FName MaterialParameterName;

	UPROPERTY(EditAnywhere, Category = Appearance)
		TSubclassOf<AGraphEdgeMeshActor> MeshClass;

	//Save Game
	UPROPERTY(SaveGame)
		int32 Id;
private:
	
	void InitMeshes();
	AGraphEdgeMeshActor* SpawnAndPlaceMeshActor();
	void UpdateMeshActorPosition();
	UPROPERTY(Transient)
	AGraphEdgeMeshActor* MeshActor;

	UPROPERTY(Transient)
		UTextRenderComponent* Text;

	UPROPERTY(Transient)
		UMaterialInstanceDynamic* Material;

	FVector InitialWorldLocation;
	float Length;
	FLinearColor CurrentColor;

	
};