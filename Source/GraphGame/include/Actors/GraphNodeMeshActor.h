// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphNodeMeshActor.generated.h"

class UGraphNode;


UCLASS()
class GRAPHGAME_API AGraphNodeMeshActor : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UGraphNode* GraphNode = nullptr;
};