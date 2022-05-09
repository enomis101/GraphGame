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
#include "Save/GraphSaveManager.h"
#include "Save/GraphSaveGame.h"
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
	InputComponent->BindAction("RunAlgorithm", IE_Pressed, this, &ThisClass::RunAlgorithm);
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
	if (GameGraph)
	{
		GameGraph->OnMouseClick();
	}
}

void AGameplayPlayerController::OnMouseReleased()
{
	if (GameGraph)
	{
		GameGraph->OnMouseReleased();
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

void AGameplayPlayerController::RunAlgorithm()
{
	ensure(GameGraph);
	GameGraph->RunAlgorithm();
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
	FString UnlitCommand = FString("viewmode unlit");
	ConsoleCommand(UnlitCommand);

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

	GraphSaveManager = NewObject<UGraphSaveManager>(this, UGraphSaveManager::StaticClass());
	LoadGame();
}

void AGameplayPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveGame();
}

void AGameplayPlayerController::SaveGame()
{
	if (UGraphSaveGame* SaveGameInstance = Cast<UGraphSaveGame>(UGameplayStatics::CreateSaveGameObject(UGraphSaveGame::StaticClass())))
	{
		SaveGameInstance->SaveVersion = UGraphSaveGame::CurrentSaveVersion;
		ensure(GraphSaveManager);
		// Set data on the savegame object.
		GraphSaveManager->GetSaveData(SaveGameInstance->MemoryBuffer);

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0 /*UserIndex*/))
		{
			// The operation was successful, so LoadedGame now contains the data we saved earlier.
			UE_LOG(LogTemp, Warning, TEXT("save: GRAPHS SAVE SUCCESS"));
		}
	}
}

void AGameplayPlayerController::LoadGame()
{
	// Retrieve and cast the USaveGame object to UMySaveGame.
	if (UGraphSaveGame* LoadedGame = Cast<UGraphSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
	{
		// The operation was successful, so LoadedGame now contains the data we saved earlier.
		UE_LOG(LogTemp, Warning, TEXT("save: GRAPHS LOADED"));

		ensure(GraphSaveManager);
		if (LoadedGame->SaveVersion == UGraphSaveGame::CurrentSaveVersion)
		{
			GraphSaveManager->LoadSaveData(LoadedGame->MemoryBuffer);
		}
	}
}

void AGameplayPlayerController::SaveCurrentGraphAs(const FString& GraphName)
{
	ensure(GraphSaveManager);
	if (GameGraph)
	{
		GameGraph->GraphName = GraphName;
		GraphSaveManager->SaveGraph(GameGraph);
	}
}

void AGameplayPlayerController::LoadGraph(const FString& GraphName)
{
	ensure(GraphSaveManager);
	UGraph* NewGraph = GraphSaveManager->GetSavedGraph(GraphName);
	LoadGraph(NewGraph);
}

void AGameplayPlayerController::LoadGraph(UGraph* GraphToLoad)
{
	ensure(GraphSaveManager && GraphSaveManager->SavedGraphs.Contains(GraphToLoad));
	if (GraphToLoad)
	{
		if (GameGraph)
		{
			GameGraph->DeInit();
		}

		GraphToLoad->Init();
		GameGraph = GraphToLoad;
	}
}

void AGameplayPlayerController::DeleteGraph(UGraph* GraphToDelete)
{
	if (GraphToDelete == GameGraph)
	{
		RemoveCurrentGraph();
	}
	GraphSaveManager->DeleteSavedGraph(GraphToDelete);
}

void AGameplayPlayerController::RemoveCurrentGraph()
{
	if (GameGraph)
	{
		GameGraph->DeInit();
		GameGraph = nullptr;
	}
}

void AGameplayPlayerController::CreateNewGraph(bool bDirected)
{
	RemoveCurrentGraph();

	GameGraph = NewObject<UGraph>(this, GameGraphClass);
	if (GameGraph)
	{
		GameGraph->Init( true /*bNewGraph*/, bDirected);
	}
}

UGraph* AGameplayPlayerController::SpawnNewGraph()
{
	return NewObject<UGraph>(this, GameGraphClass);
}
