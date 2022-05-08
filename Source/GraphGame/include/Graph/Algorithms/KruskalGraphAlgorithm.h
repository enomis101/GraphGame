// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "DataStructures/UnionFind.h"
#include "KruskalGraphAlgorithm.generated.h"

class UGraphAlgorithmParams;
class UGraphEdge;



UCLASS()
class GRAPHGAME_API UMSTKruskalGraphAlgorithm : public UGraphAlgorithm
{
	GENERATED_BODY()
public:
	virtual void Start(UGraphAlgorithmParams* InParams);
	//Return true if finished false otherwise
	virtual bool Step();
	virtual void End();

protected:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor NotMSTEdgeColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor MSTNodeColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor MSTEdgeColor;

protected:
	IntUnionFind U;
	TSet<UGraphEdge*> MST;
	TArray<UGraphEdge*> SortedEdges;
	//TSet<UGraphNode*> ProcessedNodes;

	bool bValidGraph = false;
	int32 i;
	void AddEdge(UGraphEdge* EdgeToAdd);
};
