// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "RecursiveGraphAlgorithm.generated.h"

UCLASS(Abstract)
class GRAPHGAME_API URecursiveObject : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Step() PURE_VIRTUAL(URecursiveObject::Step, return false;);
	virtual bool HasFinished() PURE_VIRTUAL(URecursiveObject::HasFinished, return false;);
	int32 CheckPoint = 0;
};
/**
 * 
 */
UCLASS(Abstract)
class GRAPHGAME_API URecursiveGraphAlgorithm : public UGraphAlgorithm
{
	GENERATED_BODY()

public:
	virtual void Start(UGraphAlgorithmParams* InParams) PURE_VIRTUAL(URecursiveGraphAlgorithm::Start, );
	virtual bool Step();
	virtual void End() PURE_VIRTUAL(URecursiveGraphAlgorithm::End, );
	virtual void Recurse(URecursiveObject* InObject);
protected:
	UPROPERTY()
	TArray<URecursiveObject*> Stack;
};
