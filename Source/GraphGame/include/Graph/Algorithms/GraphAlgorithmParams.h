#pragma once
#include "CoreMinimal.h"
#include "Graph/GraphType.h"
#include "GraphAlgorithmParams.generated.h"

class AGameGraphNode;

UCLASS()
class UGraphAlgorithmParams : public UObject
{
	GENERATED_BODY()
public:
	EAlgorithmType AlgoType = EAlgorithmType::NA;
};

UCLASS()
class GRAPHGAME_API USearchGraphAlgorithmParams : public UGraphAlgorithmParams
{
	GENERATED_BODY()
public:

	int32 StartNode;
	int32 TargetNode;
};