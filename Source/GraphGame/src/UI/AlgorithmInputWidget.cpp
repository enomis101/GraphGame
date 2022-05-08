// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/AlgorithmInputWidget.h"
#include "Graph/Algorithms/GraphAlgorithmParams.h"

UGraphAlgorithmParams* UAlgorithmInputWidget::GetGraphAlgorithmParams()
{
	UGraphAlgorithmParams* AlgoParams = NewObject<UGraphAlgorithmParams>(GetOwningPlayer(), UGraphAlgorithmParams::StaticClass());
	if (AlgoParams)
	{
		AlgoParams->AlgoType = AlgoType;
	}
	return AlgoParams;
}
