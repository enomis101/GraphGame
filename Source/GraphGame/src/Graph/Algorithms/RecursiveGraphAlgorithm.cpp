// Fill out your copyright notice in the Description page of Project Settings.
#include "Graph/Algorithms/RecursiveGraphAlgorithm.h"
#pragma optimize("",off)

bool URecursiveGraphAlgorithm::Step()
{
	if (Stack.Num() == 0)
	{
		return true;
	}
	int32 LastIndex = Stack.Num() - 1;
	if (Stack[LastIndex]->Step())
	{
		return true;
	}

	if (Stack[LastIndex]->HasFinished())
	{
		//Pop
		Stack[LastIndex]->ConditionalBeginDestroy();
		Stack.RemoveAt(LastIndex);
	}
	return false;
}

void URecursiveGraphAlgorithm::Recurse(URecursiveObject* InObject)
{
	//Push
	Stack.Add(InObject);
}
