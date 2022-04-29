// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "BFSGraphAlgorithm.generated.h"

class UGraphAlgorithmParams;



UCLASS()
class GRAPHGAME_API UBFSGraphAlgorithm : public UGraphAlgorithm
{
	GENERATED_BODY()
public:
	virtual void Start(UGraphAlgorithmParams* InParams);
	//Return true if finished false otherwise
	virtual bool Step();
	virtual void End();

protected:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor ExploredColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ProcessedColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor FoundColor;

protected:
	TQueue<UGraphNode*> Queue;
	TSet<UGraphNode*> ExploredNodes;
	//TSet<UGraphNode*> ProcessedNodes;

	UPROPERTY(Transient)
	UGraphNode* FoundNode;

	int32 Target;
};
