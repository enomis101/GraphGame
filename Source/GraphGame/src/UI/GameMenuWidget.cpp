// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenuWidget.h"
#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include <Components/Button.h>
#include <Components/ComboBoxString.h>
#include "Graph/GameGraph.h"
#include <Misc/EnumRange.h>
#include "UI/AlgorithmInputWidget.h"
#include <Components/CanvasPanel.h>
#include "Graph/GameGraph.h"
#include <Components/EditableText.h>

void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCloseButtonClicked);

	ResetGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnResetGraphButtonClicked);

	StartButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStartButtonClicked);

	for (EAlgorithmType Val : TEnumRange<EAlgorithmType>())
	{
		FString EnumName = UEnum::GetValueAsString(Val);
		EnumName.RemoveFromStart("EAlgorithmType::", ESearchCase::Type::IgnoreCase);
		AlgorithmTypeComboBox->AddOption(EnumName);
	}
	AlgorithmTypeComboBox->OnSelectionChanged.AddUniqueDynamic(this, &ThisClass::OnAlgorithmSelection);


	for (UWidget* Widget : MainCanvas->GetAllChildren())
	{
		ensure(Widget);
		if (UAlgorithmInputWidget* AlgoInputWidget = Cast<UAlgorithmInputWidget>(Widget))
		{
			AlgoInputWidgets.Add(AlgoInputWidget);
		}
	}
}

void UGameMenuWidget::NativeDestruct()
{
	CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::OnCloseButtonClicked);

	ResetGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnResetGraphButtonClicked);

	StartButton->OnClicked.RemoveDynamic(this, &ThisClass::OnStartButtonClicked);

	AlgorithmTypeComboBox->OnSelectionChanged.RemoveDynamic(this, &ThisClass::OnAlgorithmSelection);

	Super::NativeDestruct();
}

void UGameMenuWidget::OnCloseButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		PC->SwitchToGame();
	}
}

void UGameMenuWidget::OnResetGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	UGraph* Graph = PC ? PC->GetGameGraph() : nullptr;
	if (Graph)
	{
		Graph->ResetAppearance();
	}
}

void UGameMenuWidget::OnStartButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC && AlgorithmTypeComboBox && CurrSelectedAlgoInput)
	{
		UGraphAlgorithmParams* AlgoParams = CurrSelectedAlgoInput->GetGraphAlgorithmParams();
		CurrSelectedAlgoInput->SetVisibility(ESlateVisibility::Hidden);
		CurrSelectedAlgoInput = nullptr;
		if (AlgoParams)
		{
			PC->StartAlgorithmFromMenu(AlgoParams);
		}

	}
}



void UGameMenuWidget::OnAlgorithmSelection(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, SelectedItem);
	if (AlgorithmTypeComboBox)
	{
		int i = AlgorithmTypeComboBox->GetSelectedIndex();
		EAlgorithmType SelType = static_cast<EAlgorithmType>(i);
		if (CurrSelectedAlgoInput && CurrSelectedAlgoInput->GetAlgorithmType() == SelType)
		{
			return;
		}
		if (CurrSelectedAlgoInput)
		{
			CurrSelectedAlgoInput->SetVisibility(ESlateVisibility::Hidden);
			CurrSelectedAlgoInput = nullptr;
		}
		for (UAlgorithmInputWidget* InputWidget : AlgoInputWidgets)
		{
			if (InputWidget->GetAlgorithmType() == SelType)
			{
				CurrSelectedAlgoInput = InputWidget;
				CurrSelectedAlgoInput->SetVisibility(ESlateVisibility::Visible);
			}
		}
		
	}
	
}

