// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "GameplayPlayerCameraManager.generated.h"

class ACameraActor;

/**
 * 
 */
UCLASS()
class GRAPHGAME_API AGameplayPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	FORCEINLINE ACameraActor* GetCamera() { return AnimCameraActor; }

};
