// Fill out your copyright notice in the Description page of Project Settings.
#include "UI/GraphListEntryWidget.h"
#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include "Graph/GameGraph.h"
#include <Components/TextBlock.h>
#include <Components/Button.h>

void UGraphListEntryWidget::NativeOnListItemObjectSet(UObject* Item)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(Item);

	UGraph* Graph = Cast<UGraph>(Item);
	if (Graph)
	{
		FString GraphString = Graph->GraphName + "-" + (Graph->IsDirected() ? "D" : "U");
		GraphName->SetText(FText::FromString(GraphString));
	}

	LoadGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnLoadGraphButtonClicked);
	DeleteGraphButton->OnClicked.AddUniqueDynamic(this, &ThisClass::OnDeleteGraphButtonClicked);
}

void UGraphListEntryWidget::NativeDestruct()
{
	LoadGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnLoadGraphButtonClicked);
	DeleteGraphButton->OnClicked.RemoveDynamic(this, &ThisClass::OnDeleteGraphButtonClicked);
	Super::NativeDestruct();
}

void UGraphListEntryWidget::OnLoadGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	UGraph* Graph = GetListItem<UGraph>();
	if (PC && Graph)
	{
		PC->LoadGraph(Graph);
	}
}

void UGraphListEntryWidget::OnDeleteGraphButtonClicked()
{
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	UGraph* Graph = GetListItem<UGraph>();
	if (PC && Graph)
	{
		PC->DeleteGraph(Graph);
	}
}
