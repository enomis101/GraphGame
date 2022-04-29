#include "Save/GraphSaveManager.h"
#include <Kismet/GameplayStatics.h>
#include "Gameplay/PlayerController/GameplayPlayerController.h"

UGraphSaveManager* UGraphSaveManager::Get(const UObject* WorldContextObject)
{
	AGameplayPlayerController* PC = Cast<AGameplayPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0 /*PlayerIndex*/));
	if (PC)
	{
		return PC->GetGraphSaveManager();
	}
	return nullptr;
}

void UGraphSaveManager::SaveGraph(FName GraphName, UGraph* GraphToSave)
{
	if (!SavedGraphs.Contains(GraphName))
	{
		SavedGraphs.Add(GraphName, GraphToSave);
	}
	else
	{
		SavedGraphs[GraphName] = GraphToSave;
	}
}

bool UGraphSaveManager::DeleteSavedGraph(FName GraphName)
{
	if (SavedGraphs.Contains(GraphName))
	{
		SavedGraphs.Remove(GraphName);
		return true;
	}
	return false;
}

UGraph* UGraphSaveManager::GetSavedGraph(FName GraphName)
{
	if (SavedGraphs.Contains(GraphName))
	{
		return SavedGraphs[GraphName];
	}
	return nullptr;
}

void UGraphSaveManager::GetSaveData(TMap<FName, UGraph*>& OutSaveData)
{
	OutSaveData = SavedGraphs;
}

void UGraphSaveManager::LoadSaveData(const TMap<FName, UGraph*>& InSaveData)
{
	SavedGraphs = InSaveData;
}
