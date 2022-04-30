// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphSaveWidget.generated.h"

class UButton;
class UEditableText;
class UListView;
class UCheckBox;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphSaveWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	virtual void NativeDestruct() override;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* GraphDirectedCheckBox = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* CreateNewGraphButton;
	UFUNCTION()
	void OnCreateNewGraphButtonClicked();

	UPROPERTY(meta = (BindWidget))
	UButton* RemoveCurrentGraphButton;
	UFUNCTION()
	void OnRemoveCurrentGraphButtonClicked();

	UFUNCTION()
	void OnGraphAdded(UGraph* NewGraph);

	UFUNCTION()
	void OnGraphRemoved(UGraph* RemovedGraph);

	UPROPERTY(meta = (BindWidget))
	UEditableText* GraphNameText;

	UPROPERTY(meta = (BindWidget))
	UListView* GraphList;
};
