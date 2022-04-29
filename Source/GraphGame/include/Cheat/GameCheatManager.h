// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "GameCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class GRAPHGAME_API UGameCheatManager : public UCheatManager
{
	GENERATED_BODY()
	
// 	UFUNCTION(exec)
// 	void StartAlgorithm(int32 Target);

	UFUNCTION(exec)
	void StepAlgorithm();
};
