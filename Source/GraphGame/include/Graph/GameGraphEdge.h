// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/GraphEdgeMeshActor.h"
#include "GameGraphEdge.generated.h"

struct FPropertyChangedEvent;
class AGameGraphNode;
class UStaticMeshComponent;
class UTextRenderComponent;
class UGraphNode;

UCLASS()
class GRAPHGAME_API AGameGraphEdge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameGraphEdge();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient)
	AGameGraphNode* FirstNode = nullptr;

	UPROPERTY(Transient)
	AGameGraphNode* SecondNode = nullptr;

	void Init(AGameGraphNode* N1, AGameGraphNode* N2, int32 InId);

	void OffsetRight();
	void ResetOffset();

	FString GetEdgeName();
	float GetWeight() { return Length; }
	void SetSelected();
	void Reset();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	float ZScale = 20.f;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	float RightOffsetAmount = 20.f;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	FName SelParameterName = "Selected";

	UPROPERTY(VisibleAnywhere)
	bool bBidirectional = false;
private:
	void SetEdgeName();
	void InitMeshes();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShaftMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HeadMesh;

	UPROPERTY(EditAnywhere)
	UTextRenderComponent* TextComponent;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* Material;

	FVector InitialWorldLocation;
	
	int32 Id;
	float Length;
};



UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphEdge : public UObject
{
	GENERATED_BODY()

public:

	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(Transient)
	UGraphNode* FirstNode = nullptr;

	UPROPERTY(Transient)
	UGraphNode* SecondNode = nullptr;

	void Init(UGraphNode* N1, UGraphNode* N2, int32 InId);
	void DeInit();

	float GetWeight() { return Length; }

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

	int32 Id;
	float Length;
};