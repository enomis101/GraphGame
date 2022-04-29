// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GraphEdgeMeshActor.generated.h"

class UGraphEdge;


UCLASS()
class GRAPHGAME_API AGraphEdgeMeshActor : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY()
	UGraphEdge* GraphEdge = nullptr;
};