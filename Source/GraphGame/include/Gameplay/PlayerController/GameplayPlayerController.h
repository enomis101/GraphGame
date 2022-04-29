// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Graph/GameGraph.h"
#include <Blueprint/UserWidget.h>
#include "GameplayPlayerController.generated.h"

class UCameraComponent;
class ACameraActor;
class UDebugDataAsset;
class UGraph;

/**
 * 
 */
UCLASS()
class GRAPHGAME_API AGameplayPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void SetupInputComponent() override;

	//Input functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ZoomCamera(float AxisValue);
	void OnMouseClick();
	void OnMouseReleased();
	FORCEINLINE UCameraComponent* GetCameraComponent() { return CameraComponent; }
	FORCEINLINE	AGameGraph* GetGameGraph() { return GameGraph; }
	//Algorithms
	void StartAlgorithm(UGraphAlgorithmParams* InParams);
	void StepAlgorithm();

	void HandleMenu();
	void SwitchToMenu();
	void SwitchToGame();

	void StartAlgorithmFromMenu(UGraphAlgorithmParams* AlgoParams);
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float CameraSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	float ZoomSpeed = 1.f;

	UPROPERTY(EditAnywhere)
	FVector InitialCameraLoc = FVector(0, 0, 10);

	UPROPERTY(EditAnywhere)
	FVector2D FOVRange = FVector2D(60.f,120.f);

	UPROPERTY(EditAnywhere , Category = GameGraph)
	TSubclassOf<AGameGraph> GameGraphClass;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	TSubclassOf<UGraph> GameGraphClass2;


	UPROPERTY(EditAnywhere, Category = Debug)
	UDebugDataAsset* DebugDataAsset;

	UPROPERTY(EditAnywhere, Category = GameGraph)
	TSubclassOf<UUserWidget> MenuClass;

private:
	UPROPERTY(Transient)
	UCameraComponent* CameraComponent;

	UPROPERTY(Transient)
	ACameraActor* GameCamera;

	UPROPERTY(Transient)
	AGameGraph* GameGraph;

	UPROPERTY(Transient)
	UGraph* GameGraph2;

	UPROPERTY(Transient)
	UUserWidget* Menu;

};
