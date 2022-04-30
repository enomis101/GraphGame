// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Types/SlateEnums.h>
#include "GameMenuWidget.generated.h"

class UButton;
class UComboBoxString;
class UAlgorithmInputWidget;
class UCanvasPanel;
class UEditableText;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnCloseButtonClicked();
	UFUNCTION()
	void OnResetGraphButtonClicked();
	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnAlgorithmSelection(FString SelectedItem, ESelectInfo::Type SelectionType);
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ResetGraphButton;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* AlgorithmTypeComboBox;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MainCanvas;

private:
	TArray< UAlgorithmInputWidget*> AlgoInputWidgets;

	UPROPERTY(Transient)
	UAlgorithmInputWidget* CurrSelectedAlgoInput;
};
