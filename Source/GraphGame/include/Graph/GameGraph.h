// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Graph/GameGraphNode.h"
#include "Graph/GameGraphEdge.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "Graph/GraphType.h"
#include "GameGraph.generated.h"

class AGameplayPlayerController;
class UCameraComponent;
class UBFSGraphAlgorithmParams;



UCLASS()
class GRAPHGAME_API AGameGraph : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGameGraph();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnMouseClick();
	void OnMouseReleased();

	void Reset();

	//Algorithms Functions
	void StartAlgorithm(UGraphAlgorithmParams* InParams);
	void StepAlgorithm();
	void DestroyCurrentAlgorithm();
	AGameGraphEdge* GetEdge(int32 Node1Index, int32 Node2Index);
	AGameGraphEdge* GetEdge(AGameGraphNode* Node1, AGameGraphNode* Node2);
	AGameGraphNode* GetNodeAtIndex(int32 Index, bool bFallbackLastClicked = false);
	int32 GetNodeIndexByValue(int32 Val);
	int32 GetNodeIndex(AGameGraphNode* InNode);
	//Public members
	TArray<AGameGraphNode*> Nodes;

	void SetDirected(bool bInValue);
	FORCEINLINE bool GetIsDirected() { return bIsDirected; }
private:
	bool GetPointOnZeroPlane(FVector& OutVector);
	AGameGraphNode* SpawnNewNode(const FVector& SpawnLocation);
	void SpawnNewEdge(AGameGraphNode* Node1, AGameGraphNode* Node2);
	void SpawnDirectedEdge(AGameGraphNode* Node1, AGameGraphNode* Node2);
	void SpawnUnDirectedEdge(AGameGraphNode* Node1, AGameGraphNode* Node2);
	AGameGraphEdge* SpawnAndPlaceEdge(AGameGraphNode* Node1, AGameGraphNode* Node2, bool bDirectedEdge = true);

	bool CheckValidSpawnPoint(const FVector& SpawnLocation, FHitResult& OutHitResult);
	void PrintGraph();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	TSubclassOf<AGameGraphNode> NodeClass;


	UPROPERTY(EditAnywhere, Category = GameGraph)
	TSubclassOf<AGameGraphEdge> DirectedEdgeClass;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	TSubclassOf<AGameGraphEdge> UnDirectedEdgeClass;

	UPROPERTY(Transient)
	AGameplayPlayerController* PC;

	UPROPERTY(Transient)
	UCameraComponent* CameraComponent;

	UPROPERTY(Transient)
	AGameGraphNode* LastClickedNode;


	int32 nNodes = 0;
	TMap<AGameGraphNode*, TMap<AGameGraphNode*,AGameGraphEdge*>> Edges;
	TArray< AGameGraphEdge*> EdgeList;
	int32 nEdges = 0;

	//Algorithms Members
	UPROPERTY(Transient)
	UGraphAlgorithm* CurrentRunningAlgorithm = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TMap<EAlgorithmType, TSubclassOf<UGraphAlgorithm>> AlgorithmClasses;

	bool bIsDirected = true;
};


UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraph : public UObject
{
	GENERATED_BODY()

public:
	void OnMouseClick();
	void OnMouseReleased();

	void ResetAppearance();

	//Algorithms Functions
	void StartAlgorithm(UGraphAlgorithmParams* InParams);
	void StepAlgorithm();
	void DestroyCurrentAlgorithm();
	UGraphEdge* GetEdge(int32 Node1Index, int32 Node2Index);
	UGraphEdge* GetEdge(UGraphNode* Node1, UGraphNode* Node2);
	UGraphNode* GetNodeAtIndex(int32 Index, bool bFallbackLastClicked = false);
	int32 GetNodeIndexByValue(int32 Val);
	int32 GetNodeIndex(UGraphNode* InNode);
	//Public members

	void Init();

	UPROPERTY()
	TArray<UGraphNode*> Nodes;

	void SetDirected(bool bInValue);
	FORCEINLINE bool GetIsDirected() { return bIsDirected; }
private:
	bool GetPointOnZeroPlane(FVector& OutVector);
	UGraphNode* SpawnNewNode(const FVector& SpawnLocation);
	void SpawnNewEdge(UGraphNode* Node1, UGraphNode* Node2);
	void SpawnDirectedEdge(UGraphNode* Node1, UGraphNode* Node2);
	void SpawnUnDirectedEdge(UGraphNode* Node1, UGraphNode* Node2);


	bool IsValidSpawnPoint(const FVector& SpawnLocation, FHitResult& OutHitResult);
	void PrintGraph();
protected:

	UPROPERTY(EditAnywhere, Category = GameGraph)
		TSubclassOf<UGraphNode> NodeClass;

	UPROPERTY(EditAnywhere, Category = GameGraph)
		TSubclassOf<AActor> TestNodeClass;

	UPROPERTY(EditAnywhere, Category = GameGraph)
		TSubclassOf<UGraphEdge> DirectedEdgeClass;

	UPROPERTY(EditAnywhere, Category = GameGraph)
		TSubclassOf<UGraphEdge> UnDirectedEdgeClass;

	UPROPERTY(Transient)
		AGameplayPlayerController* PC;

	UPROPERTY(Transient)
		UCameraComponent* CameraComponent;

	UPROPERTY(Transient)
		UGraphNode* LastClickedNode;


	int32 nNodes = 0;

	TMap<UGraphNode*, TMap<UGraphNode*, UGraphEdge*>> Edges;

	UPROPERTY()
	TArray< UGraphEdge*> EdgeList;
	int32 nEdges = 0;

	//Algorithms Members
	UPROPERTY(Transient)
		UGraphAlgorithm* CurrentRunningAlgorithm = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TMap<EAlgorithmType, TSubclassOf<UGraphAlgorithm>> AlgorithmClasses;

	bool bIsDirected = true;
};