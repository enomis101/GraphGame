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
#include <Components/CheckBox.h>

void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (CloseButton)
	{
		CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCloseButtonClicked);
	}
	if (ResetGraphButton)
	{
		ResetGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnResetGraphButtonClicked);
	}
	if (StartButton)
	{
		StartButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStartButtonClicked);
	}

	if (GraphTypeCheckBox)
	{
		AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
		if (PC)
		{
			GraphTypeCheckBox->SetCheckedState(PC->GetGameGraph()->GetIsDirected() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
		}
		GraphTypeCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &ThisClass::OnGraphTypeCheckBoxChanged);
	}
	if (AlgorithmTypeComboBox)
	{
		for (EAlgorithmType Val : TEnumRange<EAlgorithmType>())
		{
			FString EnumName =  UEnum::GetValueAsString(Val);
			EnumName.RemoveFromStart("EAlgorithmType::", ESearchCase::Type::IgnoreCase);
			AlgorithmTypeComboBox->AddOption(EnumName);
		}
		AlgorithmTypeComboBox->OnSelectionChanged.AddUniqueDynamic(this, &ThisClass::OnAlgorithmSelection);
	}
	if (MainCanvas)
	{
		for (UWidget* Widget : MainCanvas->GetAllChildren())
		{
			ensure(Widget);
			if (UAlgorithmInputWidget* AlgoInputWidget = Cast<UAlgorithmInputWidget>(Widget))
			{
				AlgoInputWidgets.Add(AlgoInputWidget);
			}
		}
	}
}

void UGameMenuWidget::NativeDestruct()
{
	if (CloseButton)
	{
		CloseButton->OnClicked.RemoveDynamic(this, &ThisClass::OnCloseButtonClicked);
	}
	if (ResetGraphButton)
	{
		ResetGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnResetGraphButtonClicked);
	}
	if (StartButton)
	{
		StartButton->OnClicked.RemoveDynamic(this, &ThisClass::OnStartButtonClicked);
	}
	if (AlgorithmTypeComboBox)
	{
		AlgorithmTypeComboBox->OnSelectionChanged.RemoveDynamic(this, &ThisClass::OnAlgorithmSelection);
	}
	if (GraphTypeCheckBox)
	{
		GraphTypeCheckBox->OnCheckStateChanged.RemoveDynamic(this, &ThisClass::OnGraphTypeCheckBoxChanged);
	}
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
	AGameGraph* Graph = PC ? PC->GetGameGraph() : nullptr;
	if (Graph)
	{
		Graph->Reset();
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

void UGameMenuWidget::OnGraphTypeCheckBoxChanged(bool bIsChecked)
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		PC->GetGameGraph()->SetDirected(bIsChecked);
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
