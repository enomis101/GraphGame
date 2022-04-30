
#pragma once

#include "CoreMinimal.h"
#include "GraphSaveManager.generated.h"

class UGraph;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSaveDataLoadedDynamic);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGraphSaveManagerEventDynamic, UGraph*, Item);

UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphSaveManager : public UObject
{
	GENERATED_BODY()

public:
	static UGraphSaveManager* Get(const UObject* WorldContextObject);
	void SaveGraph(UGraph* GraphToSave);
	bool DeleteSavedGraph(const FString& GraphName);
	UGraph* GetSavedGraph(const FString& GraphName);

	void GetSaveData(TArray<uint8>& OutSaveData);
	void LoadSaveData(const TArray<uint8>& InSaveData);
	UPROPERTY()
	TSet<UGraph*> SavedGraphs;

	FGraphSaveManagerEventDynamic OnGraphAdded;
	FGraphSaveManagerEventDynamic OnGraphRemoved;
	FOnSaveDataLoadedDynamic OnSaveDataLoaded;
};