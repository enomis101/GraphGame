#include "Save/GraphSaveManager.h"
#include <Kismet/GameplayStatics.h>
#include "Gameplay/PlayerController/GameplayPlayerController.h"
#include <Serialization/MemoryWriter.h>
#include <Serialization/MemoryReader.h>

UGraphSaveManager* UGraphSaveManager::Get(const UObject* WorldContextObject)
{
	AGameplayPlayerController* PC = Cast<AGameplayPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, 0 /*PlayerIndex*/));
	if (PC)
	{
		return PC->GetGraphSaveManager();
	}
	return nullptr;
}

void UGraphSaveManager::SaveGraph(UGraph* GraphToSave)
{
	if (!SavedGraphs.Contains(GraphToSave))
	{
		SavedGraphs.Add(GraphToSave);
		OnGraphAdded.Broadcast(GraphToSave);
	}
}

bool UGraphSaveManager::DeleteSavedGraph(const FString& GraphName)
{
	UGraph* GraphToRemove = nullptr;
	for (UGraph* Graph : SavedGraphs)
	{
		if (Graph->GraphName == GraphName)
		{
			GraphToRemove = Graph;
			break;
		}
	}
	DeleteSavedGraph(GraphToRemove);
	return false;
}

bool UGraphSaveManager::DeleteSavedGraph(UGraph* GraphToDelete)
{
	if (GraphToDelete && SavedGraphs.Contains(GraphToDelete))
	{
		SavedGraphs.Remove(GraphToDelete);
		OnGraphRemoved.Broadcast(GraphToDelete);
		return true;
	}
	return false;
}

UGraph* UGraphSaveManager::GetSavedGraph(const FString& GraphName)
{
	for (UGraph* Graph : SavedGraphs)
	{
		if (Graph->GraphName == GraphName)
		{
			return Graph;
		}
	}
	return nullptr;
}

void UGraphSaveManager::GetSaveData(TArray<uint8>& OutSaveData)
{
	OutSaveData.Empty();
	FMemoryWriter Writer(OutSaveData);
	int32 GrapsNum = SavedGraphs.Num();
	Writer << GrapsNum;
	for (UGraph* Graph : SavedGraphs)
	{
		Graph->SerializeGraph(Writer);
	}
}

void UGraphSaveManager::LoadSaveData(const TArray<uint8>& InSaveData)
{
	FMemoryReader Reader(InSaveData);
	int32 GraphsNum = 0;
	Reader << GraphsNum;
	SavedGraphs.Empty();
	AGameplayPlayerController* PC = Cast<AGameplayPlayerController>(GetOuter());
	ensure(PC);
	for (int32 i = 0; i < GraphsNum; i++)
	{
		UGraph* NewGraph = PC->SpawnNewGraph();
		SavedGraphs.Add(NewGraph);
		NewGraph->SerializeGraph(Reader);
	}
	OnSaveDataLoaded.Broadcast();
}
