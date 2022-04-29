// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DebugDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHGAME_API UDebugDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = Debug)
	float SphereRadius = 50.f;
};
