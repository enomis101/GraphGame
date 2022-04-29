// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "DFSIterativeGraphAlgorithm.generated.h"

class UGraphAlgorithmParams;



UCLASS()
class GRAPHGAME_API UDFSIterativeGraphAlgorithm : public UGraphAlgorithm
{
	GENERATED_BODY()
public:
	virtual void Start(UGraphAlgorithmParams* InParams);
	//Return true if finished false otherwise
	virtual bool Step();
	virtual void End();

	void SetNodeExplored(UGraphNode* InNode);
protected:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor ExploredColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor ProcessedColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor FoundColor;

protected:
	TArray<UGraphNode*> Stack;
	TSet<UGraphNode*> ExploredNodes;

	UPROPERTY(Transient)
	UGraphNode* FoundNode;

	int32 Target;
};
