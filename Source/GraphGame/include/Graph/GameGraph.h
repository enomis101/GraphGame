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

	//UGraph(const FObjectInitializer& ObjectInitializer);
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
	UGraphNode* GetNodeById(int32 NodeId);
	int32 GetNodeIndexByValue(int32 Val);
	int32 GetNodeIndex(UGraphNode* InNode);
	//Public members

	void Init(bool bNewGraph = false, bool bIsDir = true);
	void DeInit();

	void SerializeGraph(FArchive& Archive);

	UPROPERTY(SaveGame)
	FString GraphName;

	UPROPERTY(SaveGame)
	TArray<UGraphNode*> Nodes;
	UPROPERTY(SaveGame)
	TArray< UGraphEdge*> Edges;

	FORCEINLINE int32 GetNodesNum() { return Nodes.Num(); }
	FORCEINLINE int32 GetEdgesNum() { return Edges.Num(); }
	FORCEINLINE bool IsDirected() { return bIsDirected; }
private:
	bool GetPointOnZeroPlane(FVector& OutVector);
	UGraphNode* SpawnNewNode(const FVector& SpawnLocation);
	void SpawnNewEdge(UGraphNode* Node1, UGraphNode* Node2);
	void SpawnDirectedEdge(UGraphNode* Node1, UGraphNode* Node2);
	void SpawnUnDirectedEdge(UGraphNode* Node1, UGraphNode* Node2);


	bool IsValidSpawnPoint(const FVector& SpawnLocation, FHitResult& OutHitResult);
	void PrintGraph();
protected:
	//virtual void BeginDestroy() override;

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
		bool bIsDirected = true;

	//Algorithms Members
	UPROPERTY(Transient)
		UGraphAlgorithm* CurrentRunningAlgorithm = nullptr;

	UPROPERTY(EditDefaultsOnly)
		TMap<EAlgorithmType, TSubclassOf<UGraphAlgorithm>> AlgorithmClasses;


};