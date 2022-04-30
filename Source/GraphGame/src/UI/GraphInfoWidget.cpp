// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GraphInfoWidget.h"
#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include <Components/TextBlock.h>
#include "Graph/GameGraph.h"


void UGraphInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	CurrGraph = PC->GetGameGraph();
	UpdateGraphInfo();
}

void UGraphInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	AGameplayPlayerController* PC = GetOwningPlayer<AGameplayPlayerController>();
	if (CurrGraph != PC->GetGameGraph())
	{
		CurrGraph = PC->GetGameGraph();
		UpdateGraphInfo();
	}
}

void UGraphInfoWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGraphInfoWidget::UpdateGraphInfo()
{
	GraphNameText->SetVisibility(CurrGraph ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	NodesNumText->SetVisibility(CurrGraph ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	EdgesNumText->SetVisibility(CurrGraph ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	if (!CurrGraph)
	{
		return;
	}
	FString GraphNameString = "Name: " + CurrGraph->GraphName + "-" + (CurrGraph->IsDirected() ? "DIR" : "UNDIR");
	GraphNameText->SetText(FText::FromString(GraphNameString));

	FString NodesNumString = "Nodes Num.: " + FString::FromInt(CurrGraph->GetNodesNum());
	NodesNumText->SetText(FText::FromString(NodesNumString));

	FString EdgesNumString = "Edges Num.: " + FString::FromInt(CurrGraph->GetEdgesNum());
	EdgesNumText->SetText(FText::FromString(EdgesNumString));
}
