// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphInfoWidget.generated.h"

class UTextBlock;
class UGraph;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GraphNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NodesNumText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* EdgesNumText;

	UPROPERTY(Transient)
	UGraph* CurrGraph = nullptr;

private:
	void UpdateGraphInfo();
};
