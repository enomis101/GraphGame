// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Graph/Algorithms/RecursiveGraphAlgorithm.h"
#include "DFSRecursiveGraphAlgorithm.generated.h"

UCLASS()
class GRAPHGAME_API UDFSRecursiveObject : public URecursiveObject
{
	GENERATED_BODY()

public:
	void Init(AGameGraphNode* InRootNode);

	virtual bool Step() override;
	virtual bool HasFinished() override { return CheckPoint == 2; }
	virtual void BeginDestroy() override;
protected:
	void Recurse(AGameGraphNode* RootNode);

	UPROPERTY(Transient)
	UDFSRecursiveGraphAlgorithm* Algorithm = nullptr;

	UPROPERTY(Transient)
	AGameGraphNode* RootNode;

	int32 i = 0;
};

/**
 * 
 */
UCLASS()
class GRAPHGAME_API UDFSRecursiveGraphAlgorithm : public URecursiveGraphAlgorithm
{
	GENERATED_BODY()

public:
	virtual void Start(UGraphAlgorithmParams* InParams) override;
	virtual void End() override;
	bool IsNodeExplored(AGameGraphNode* InNode);
	void SetNodeExplored(AGameGraphNode* InNode);

	void SetNodeProcessed(AGameGraphNode* InNode);
	void SetNodeFound(AGameGraphNode* InNode);
	int32 Target;

protected:
	UPROPERTY(EditDefaultsOnly)
		FLinearColor ExploredColor;

	UPROPERTY(EditDefaultsOnly)
		FLinearColor ProcessedColor;

	UPROPERTY(EditDefaultsOnly)
		FLinearColor FoundColor;

	TSet<AGameGraphNode*> ExploredNodes;

	UPROPERTY(Transient)
	AGameGraphNode* FoundNode;

	
};
