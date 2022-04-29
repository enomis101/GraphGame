// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GraphAlgorithm.generated.h"

class UGraph;
class UGraphNode;
class UGraphAlgorithmParams;


/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphAlgorithm : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(UGraph* InGraph);

	virtual void Start(UGraphAlgorithmParams* InParams) PURE_VIRTUAL(UGraphAlgorithm::Start, );
	virtual bool Step() PURE_VIRTUAL(UGraphAlgorithm::Step, return false;);
	virtual void End() PURE_VIRTUAL(UGraphAlgorithm::End, );

	virtual void SelectActiveNode(UGraphNode* Node);

	UPROPERTY(Transient)
	UGraph* Graph;
};