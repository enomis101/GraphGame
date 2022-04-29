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
#include <Components/EditableText.h>

void UGameMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCloseButtonClicked);

	ResetGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnResetGraphButtonClicked);

	StartButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnStartButtonClicked);

	SaveCurrGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnSaveCurrGraphButtonClicked);
	LoadGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnLoadGraphButtonClicked);
	CreateNewGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreateNewGraphButtonClicked);
	RemoveCurrentGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnRemoveCurrentGraphButtonClicked);

	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC && PC->GetGameGraph())
	{
		GraphTypeCheckBox->SetCheckedState(PC->GetGameGraph()->GetIsDirected() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
	}
	else
	{
		GraphTypeCheckBox->SetCheckedState(ECheckBoxState::Undetermined);
	}
	GraphTypeCheckBox->OnCheckStateChanged.AddUniqueDynamic(this, &ThisClass::OnGraphTypeCheckBoxChanged);


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

	GraphTypeCheckBox->OnCheckStateChanged.RemoveDynamic(this, &ThisClass::OnGraphTypeCheckBoxChanged);

	SaveCurrGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnSaveCurrGraphButtonClicked);
	LoadGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnLoadGraphButtonClicked);
	CreateNewGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnCreateNewGraphButtonClicked);
	RemoveCurrentGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnRemoveCurrentGraphButtonClicked);
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

void UGameMenuWidget::OnSaveCurrGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		FText InputText = GraphNameText->GetText();
		if (!InputText.IsEmptyOrWhitespace())
		{
			PC->SaveCurrentGraphAs(FName(InputText.ToString()));
		}

	}
}

void UGameMenuWidget::OnLoadGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		FText InputText = GraphNameText->GetText();
		if (!InputText.IsEmptyOrWhitespace())
		{
			PC->LoadGraph(FName(InputText.ToString()));
		}
		
	}
}

void UGameMenuWidget::OnCreateNewGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		PC->CreateNewGraph();
	}
}

void UGameMenuWidget::OnRemoveCurrentGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		PC->RemoveCurrentGraph();
	}
}
