// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph/Algorithms/GraphAlgorithm.h"
#include "DataStructures/PriorityQueue.h"
#include "PrimGraphAlgorithm.generated.h"

class UGraphAlgorithmParams;
class UGraphEdge;
UCLASS()
class GRAPHGAME_API UMSTPrimGraphAlgorithm : public UGraphAlgorithm
{
	GENERATED_BODY()
public:
	virtual void Start(UGraphAlgorithmParams* InParams);
	//Return true if finished false otherwise
	virtual bool Step();
	virtual void End();

protected:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor NotMSTEdgeColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor MSTNodeColor;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor MSTEdgeColor;

protected:
	TMinPriorityQueue<int32> Q;
	//Nodes in the MST
	TSet<int32> NMST;
	//Edges in the MST
	TSet<UGraphEdge*> EMST;	
	//Used to get the edge between a newly added node and a node already in the MST
	TArray<int32> Parents;
	bool bValidGraph = false;

	void AddToMST(int32 NodeIndex);
	void Relax(int32 u, int32 v);
	int32 Root;
};
