// Fill out your copyright notice in the Description page of Project Settings.


#include "Cheat/GameCheatManager.h"
#include "Gameplay/PlayerController/GameplayPlayerController.h"
//
//void UGameCheatManager::StartAlgorithm(int32 Target)
//{
//	AGameplayPlayerController* PC = Cast< AGameplayPlayerController>(GetOuter());
//	if (PC)
//	{
//		PC->StartAlgorithm(Target);
//	}
//}

void UGameCheatManager::StepAlgorithm()
{
	AGameplayPlayerController* PC = Cast< AGameplayPlayerController>(GetOuter());
	if (PC)
	{
		PC->StepAlgorithm();
	}
}
