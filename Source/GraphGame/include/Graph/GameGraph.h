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
	void DeInit();

	UPROPERTY(SaveGame)
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


	TMap<UGraphNode*, TMap<UGraphNode*, UGraphEdge*>> EdgeMap;

	//SAVE
	UPROPERTY(SaveGame)
	TArray< UGraphEdge*> Edges;
	UPROPERTY(SaveGame)
		bool bIsDirected = true;

	//Algorithms Members
	UPROPERTY(Transient)
		UGraphAlgorithm* CurrentRunningAlgorithm = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TMap<EAlgorithmType, TSubclassOf<UGraphAlgorithm>> AlgorithmClasses;


};