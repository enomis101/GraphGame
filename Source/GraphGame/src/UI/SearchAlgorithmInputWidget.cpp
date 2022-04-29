// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SearchAlgorithmInputWidget.h"
#include <Misc/DefaultValueHelper.h>
#include "Graph/Algorithms/GraphAlgorithmParams.h"
#include <Components/EditableText.h>

UGraphAlgorithmParams* USearchAlgorithmInputWidget::GetGraphAlgorithmParams()
{
	int32 StartNodeIndex = -1;
	FText InputText = StartNode->GetText();
	if (!InputText.IsEmptyOrWhitespace())
	{
		FString InputString = InputText.ToString();
		FDefaultValueHelper::ParseInt(InputString, StartNodeIndex);
	}
	int32 TargetNodeIndex = -1;
	InputText = TargetNode->GetText();
	if (!InputText.IsEmptyOrWhitespace())
	{
		FString InputString = InputText.ToString();
		FDefaultValueHelper::ParseInt(InputString, TargetNodeIndex);
	}

	USearchGraphAlgorithmParams* AlgoParams = NewObject<USearchGraphAlgorithmParams>(GetOwningPlayer(), USearchGraphAlgorithmParams::StaticClass());
	if (AlgoParams)
	{
		AlgoParams->AlgoType = AlgoType;
		AlgoParams->StartNode = StartNodeIndex;
		AlgoParams->TargetNode = TargetNodeIndex;
	}
	return AlgoParams;
}
