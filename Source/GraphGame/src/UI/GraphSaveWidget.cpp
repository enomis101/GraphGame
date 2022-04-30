// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GraphSaveWidget.h"
#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include <Components/EditableText.h>
#include <Components/Button.h>
#include <Components/ListView.h>
#include "Save/GraphSaveManager.h"
#include <Components/CheckBox.h>

void UGraphSaveWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CreateNewGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnCreateNewGraphButtonClicked);
	RemoveCurrentGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnRemoveCurrentGraphButtonClicked);
	GraphDirectedCheckBox->SetCheckedState(ECheckBoxState::Checked);

	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	UGraphSaveManager* GraphSaveManager = PC ? PC->GetGraphSaveManager() : nullptr;
	if (GraphSaveManager)
	{
		for (UGraph* Graph : GraphSaveManager->SavedGraphs)
		{
			GraphList->AddItem(Graph);
		}
		GraphSaveManager->OnGraphAdded.AddUniqueDynamic(this, &ThisClass::OnGraphAdded);
		GraphSaveManager->OnGraphRemoved.AddUniqueDynamic(this, &ThisClass::OnGraphRemoved);
	}
}

void UGraphSaveWidget::NativeDestruct()
{
	CreateNewGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnCreateNewGraphButtonClicked);
	RemoveCurrentGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnRemoveCurrentGraphButtonClicked);
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	UGraphSaveManager* GraphSaveManager = PC ? PC->GetGraphSaveManager() : nullptr;
	if (GraphSaveManager)
	{
		GraphSaveManager->OnGraphAdded.RemoveDynamic(this, &ThisClass::OnGraphAdded);
		GraphSaveManager->OnGraphRemoved.RemoveDynamic(this, &ThisClass::OnGraphRemoved);
	}

	Super::NativeDestruct();
}


void UGraphSaveWidget::OnCreateNewGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		FText InputText = GraphNameText->GetText();
		if (!InputText.IsEmptyOrWhitespace())
		{
			PC->CreateNewGraph(GraphDirectedCheckBox->GetCheckedState() == ECheckBoxState::Checked);
			PC->SaveCurrentGraphAs(InputText.ToString());
		}	
	}
}

void UGraphSaveWidget::OnRemoveCurrentGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (PC)
	{
		PC->RemoveCurrentGraph();
	}
}

void UGraphSaveWidget::OnGraphAdded(UGraph* NewGraph)
{
	GraphList->AddItem(NewGraph);
}

void UGraphSaveWidget::OnGraphRemoved(UGraph* RemovedGraph)
{
	GraphList->RemoveItem(RemovedGraph);
}
