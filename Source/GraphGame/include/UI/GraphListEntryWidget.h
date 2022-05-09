// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Blueprint/IUserObjectListEntry.h>
#include "GraphListEntryWidget.generated.h"

class UButton;
class UTextBlock;


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphListEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* Item) override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GraphName;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadGraphButton;

	UPROPERTY(meta = (BindWidget))
		UButton* DeleteGraphButton;

	UFUNCTION()
	void OnLoadGraphButtonClicked();

	UFUNCTION()
		void OnDeleteGraphButtonClicked();
};
