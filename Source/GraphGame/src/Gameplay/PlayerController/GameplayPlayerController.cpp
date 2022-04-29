// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include <Camera/CameraActor.h>
#include <EngineUtils.h>
#include "Camera/GameplayPlayerCameraManager.h"
#include <Camera/CameraComponent.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <DrawDebugHelpers.h>
#include "Data/DebugDataAsset.h"
#include "Graph/GameGraphNode.h"
#include <Blueprint/WidgetBlueprintLibrary.h>
#pragma optimize("", off)

void AGameplayPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	//InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ThisClass::ZoomIn);
	//InputComponent->BindAction("ZoomIn", IE_Released, this, &ThisClass::ZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ThisClass::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ThisClass::MoveRight);
	InputComponent->BindAxis("Zoom", this, &ThisClass::ZoomCamera);

	InputComponent->BindAction("MouseClick", IE_Pressed, this, &ThisClass::OnMouseClick);
	InputComponent->BindAction("MouseClick", IE_Released, this, &ThisClass::OnMouseReleased);
	InputComponent->BindAction("StepAlgorithm", IE_Pressed, this, &ThisClass::StepAlgorithm);
	InputComponent->BindAction("Menu", IE_Pressed, this, &ThisClass::HandleMenu);
}

void AGameplayPlayerController::MoveForward(float AxisValue)
{
	if (AxisValue != 0.f && CameraComponent && GameCamera) 
	{
		//FTransform InitialTransform = CameraComponent->GetComponentTransform();
		//FVector ForwardVector = InitialTransform.TransformVector(FVector::ForwardVector);
		//FVector NewPosition = InitialTransform.GetLocation() + AxisValue * CameraSpeed * ForwardVector;
		//CameraComponent->SetWorldLocation(NewPosition);
		FTransform InitialTransform = GameCamera->GetActorTransform();
		FVector ForwardVector = InitialTransform.TransformVector(FVector::UpVector);
		FVector NewPosition = InitialTransform.GetLocation() + AxisValue * CameraSpeed * ForwardVector;
		GameCamera->SetActorLocation(NewPosition);
	}
}

void AGameplayPlayerController::MoveRight(float AxisValue)
{
	if (AxisValue != 0.f && CameraComponent)
	{
		FTransform InitialTransform = CameraComponent->GetComponentTransform();
		FVector RightVector = InitialTransform.TransformVector(FVector::RightVector);
		FVector NewPosition = InitialTransform.GetLocation() + AxisValue * CameraSpeed * RightVector;
		CameraComponent->SetWorldLocation(NewPosition);
	}
}

void AGameplayPlayerController::ZoomCamera(float AxisValue)
{
	if (AxisValue != 0.f && CameraComponent)
	{
		//float InitialFOV = CameraComponent->FieldOfView;
		//CameraComponent->SetFieldOfView(FMath::Clamp(InitialFOV + AxisValue * CameraSpeed, FOVRange.X, FOVRange.Y));
		FTransform InitialTransform = CameraComponent->GetComponentTransform();
		FVector NewPosition = InitialTransform.GetLocation() + AxisValue * ZoomSpeed * FVector::UpVector;
		CameraComponent->SetWorldLocation(NewPosition);
	}
}

void AGameplayPlayerController::OnMouseClick()
{
	if (GameGraph2)
	{
		GameGraph2->OnMouseClick();
	}
}

void AGameplayPlayerController::OnMouseReleased()
{
	if (GameGraph2)
	{
		GameGraph2->OnMouseReleased();
	}
}

void AGameplayPlayerController::StartAlgorithm(UGraphAlgorithmParams* InParams)
{
	ensure(GameGraph);
	GameGraph->StartAlgorithm(InParams);
}

void AGameplayPlayerController::StepAlgorithm()
{
	ensure(GameGraph);
	GameGraph->StepAlgorithm();
}

void AGameplayPlayerController::HandleMenu()
{
	if (Menu)
	{
		SwitchToGame();
	}
	else
	{
		SwitchToMenu();
	}
}

void AGameplayPlayerController::SwitchToMenu()
{
	Menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (Menu)
	{
		Menu->AddToViewport();
		//Menu->SetFocus();
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, Menu);

	}
}

void AGameplayPlayerController::SwitchToGame()
{
	if (Menu)
	{
		Menu->RemoveFromViewport();
		//UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
		UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this,nullptr, EMouseLockMode::DoNotLock, false);
		SetShowMouseCursor(true);
		Menu = nullptr;
		//UWidgetBlueprintLibrary::SetFocusToGameViewport();
	}
}



void AGameplayPlayerController::StartAlgorithmFromMenu(UGraphAlgorithmParams* AlgoParams)
{
	SwitchToGame();
	StartAlgorithm(AlgoParams);
}

void AGameplayPlayerController::BeginPlay()
{
	Super::BeginPlay();
	bool bResult = false;
	AGameplayPlayerCameraManager* CameraManager = Cast<AGameplayPlayerCameraManager>(PlayerCameraManager);
	AActor* Target = CameraManager ? CameraManager->GetViewTarget() : nullptr;
	if (Target) {
		// spawn the temp CameraActor used for updating CameraAnims
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = GetInstigator();
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.ObjectFlags |= RF_Transient;	// We never want to save these temp actors into a map
		GameCamera = GetWorld()->SpawnActor<ACameraActor>(SpawnInfo);


		//for (FActorIterator It(GetWorld()); It; ++It)
		//{
		//	AActor* Actor = *It;
		//	if (Actor->ActorHasTag("Camera"))
		//	{
		//		Camera = Cast<ACameraActor>(Actor);
		//	}
		//}

		CameraComponent = GameCamera->GetCameraComponent();

		//FTransform CameraTransform = GameCamera->GetActorTransform();
		//CameraTransform.SetLocation(InitialCameraLoc);
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(InitialCameraLoc, FVector::ZeroVector);
		//CameraTransform.SetRotation(FQuat(Rot));
		GameCamera->TeleportTo(InitialCameraLoc, Rot);
		CameraManager->SetViewTarget(GameCamera);
	}


	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	GameGraph = GetWorld()->SpawnActor<AGameGraph>(GameGraphClass, SpawnParameters);
	if (GameGraph)
	{
		GameGraph->SetActorHiddenInGame(true);
	}

	GameGraph2 = NewObject<UGraph>(this, GameGraphClass2);
	if (GameGraph2)
	{
		GameGraph2->Init();
	}
}
