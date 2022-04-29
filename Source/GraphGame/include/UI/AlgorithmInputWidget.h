// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/GraphType.h"
#include <Blueprint/UserWidget.h>
#include "AlgorithmInputWidget.generated.h"

class UGraphAlgorithmParams;
/**
 * 
 */
UCLASS(ABSTRACT, Blueprintable, BlueprintType)
class GRAPHGAME_API UAlgorithmInputWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	FORCEINLINE virtual EAlgorithmType GetAlgorithmType() { return EAlgorithmType::NA; }
	virtual UGraphAlgorithmParams* GetGraphAlgorithmParams() PURE_VIRTUAL(UAlgorithmInputWidget::GetGraphAlgorithmParams, return nullptr;);

};
