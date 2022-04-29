// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NoExportTypes.h"
#include "Actors/GraphNodeMeshActor.h"
#include "GameGraphNode.generated.h"

class USphereComponent;
struct FPropertyChangedEvent;
class AGameGraphEdge;
class UMaterialInstanceDynamic;
class UTextRenderComponent;

UCLASS()
class GRAPHGAME_API AGameGraphNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameGraphNode();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool ContainsEdge(int32 Node);
	void AddEdge(int32 NewEdge);

	UPROPERTY(EditDefaultsOnly)
		float SphereHitBoxRadius = 5.f;

	TArray<int32> Edges;
	void SetNodeName(int32 i);
	FString GetNodeName();
	FORCEINLINE int32 GetId() { return Id; }
	FORCEINLINE int32 GetValue() { return Id; }

	void Reset();
	void SetColor(FLinearColor InColor);
	void SetAdditionalText(const FString& InString);
	void SetDefaultColor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostEditChangeProperty(FPropertyChangedEvent& e);

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly)
	FName MaterialParameterName;
private:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereHitBox;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* Material;

	FLinearColor CurrentColor;
	int32 Id;
};



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

	TArray<int32> Edges;

	FORCEINLINE void SetSpawnLocation(FVector InSpawnLocation) { SpawnLocation = InSpawnLocation; }
	FORCEINLINE FVector GetLocation() { return SpawnLocation; }

	FORCEINLINE int32 GetId() { return Id; }
	FORCEINLINE int32 GetValue() { return Id; }

	void Init(int32 InId = -1, FVector InSpawnLocation = FVector::ZeroVector);
	void DeInit();

	//APPEARANCE

	void ResetAppearance();

	//Color
	void SetColor(FLinearColor InColor);
	void SetDefaultColor();
	
	//Text
	void SetDefaultName();
	void SetAdditionalText(const FString& InString);
	FString GetNodeName();

	
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

private:
	FLinearColor CurrentColor;
	FVector SpawnLocation;

	UPROPERTY(Transient)
	UMaterialInstanceDynamic* Material;

	UPROPERTY(Transient)
	UTextRenderComponent* Text;

	UPROPERTY(Transient)
	AGraphNodeMeshActor* MeshActor;

	int32 Id;


	void InitMesh();
};
