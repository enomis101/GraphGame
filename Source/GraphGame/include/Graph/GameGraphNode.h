// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "Actors/GraphNodeMeshActor.h"
#include "GameGraphNode.generated.h"

class USphereComponent;
struct FPropertyChangedEvent;
class UGraphEdge;
class UMaterialInstanceDynamic;
class UTextRenderComponent;


UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphNode : public UObject
{
	GENERATED_BODY()

public:
	//virtual void Tick(float DeltaTime) override;

	bool ContainsEdge(int32 Node);
	void AddEdge(int32 NewEdge);

	UPROPERTY(EditDefaultsOnly)
	float Radius = 5.f;

	UPROPERTY(SaveGame)
	TArray<int32> Edges;

	FORCEINLINE void SetSpawnLocation(FVector InSpawnLocation) { CurrentLocation = InSpawnLocation; }
	FORCEINLINE FVector GetLocation() { return CurrentLocation; }

	FORCEINLINE int32 GetId() { return Id; }
	FORCEINLINE int32 GetValue() { return Id; }

	void Init(int32 InId = -1, FVector InSpawnLocation = FVector::ZeroVector);
	void DeInit();

	void SerializeNode(FArchive& Ar);

	//APPEARANCE

	void ResetAppearance();

	//Color
	void SetColor(FLinearColor InColor);
	void SetDefaultColor();
	
	//Text
	void SetDefaultName();
	void SetAdditionalText(const FString& InString);
	FString GetNodeName();

	AGraphNodeMeshActor* GetMeshActor() { return MeshActor; }
	void MoveNode(const FVector& NewLocation);
	
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	//virtual void BeginDestroy() override;
	
	//APPEARANCE
	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		FLinearColor DefaultColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly, Category = Appearance)
		FName MaterialParameterName;

	UPROPERTY(EditAnywhere, Category = Appearance)
	TSubclassOf<AGraphNodeMeshActor> MeshClass;

	UPROPERTY(SaveGame)
	FVector CurrentLocation;

	UPROPERTY(SaveGame)
		int32 Id;

private:
	FLinearColor CurrentColor;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* Material;

	UPROPERTY(Transient)
	UTextRenderComponent* Text;

	UPROPERTY(Transient)
	AGraphNodeMeshActor* MeshActor;


	void InitMesh();
};
