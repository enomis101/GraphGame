// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "DataStructures/PriorityQueue.h"
#include "DijkstraGraphAlgorithm.generated.h"

class UGraphAlgorithmParams;
class AGameGraphNode;



UCLASS()
class GRAPHGAME_API UDijkstraGraphAlgorithm : public UGraphAlgorithm
{
	GENERATED_BODY()
public:
	virtual void Start(UGraphAlgorithmParams* InParams);
	//Return true if finished false otherwise
	virtual bool Step();
	virtual void End();

protected:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor ProcessedColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor FoundColor;

protected:
	//Map node index to node curr distance
	TArray<int32> NodeDistances;
	//Map node index to node predeccessor
	TArray<int32> NodePredecessors;

	TMinPriorityQueue<int32> PQ;
	int32 TargetIndex;

private:
	void SetEdgeSelected(int32 u, int32 v);
	void SetNodeProcessed(int32 NodeIndex);
	void SetNodeFound(int32 NodeIndex);
	void SetNodeAdditionalInfo(int32 NodeIndex);
	void Relax(int32 u, int32 v);
	bool bFoundNode = false;
};
