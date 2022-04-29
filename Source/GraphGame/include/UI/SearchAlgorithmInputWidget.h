// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/AlgorithmInputWidget.h"
#include "Graph/GraphType.h"
#include "SearchAlgorithmInputWidget.generated.h"

class UEditableText;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API USearchAlgorithmInputWidget : public UAlgorithmInputWidget
{
	GENERATED_BODY()
public:
	//Public Functions
	FORCEINLINE virtual EAlgorithmType GetAlgorithmType() override { return AlgoType; }
	virtual UGraphAlgorithmParams* GetGraphAlgorithmParams() override;

public:
	//Public Members
	UPROPERTY(meta = (BindWidget))
	UEditableText* StartNode;

	UPROPERTY(meta = (BindWidget))
	UEditableText* TargetNode;

	UPROPERTY(EditAnywhere)
	EAlgorithmType AlgoType;
};
