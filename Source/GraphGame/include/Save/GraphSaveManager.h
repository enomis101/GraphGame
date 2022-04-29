
#pragma once

#include "CoreMinimal.h"
#include "GraphSaveManager.generated.h"

class UGraph;


UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphSaveManager : public UObject
{
	GENERATED_BODY()

public:
	static UGraphSaveManager* Get(const UObject* WorldContextObject);
	void SaveGraph(FName GraphName, UGraph* GraphToSave);
	bool DeleteSavedGraph(FName GraphName);
	UGraph* GetSavedGraph(FName GraphName);

	void GetSaveData(TMap<FName, UGraph*>& OutSaveData);
	void LoadSaveData(const TMap<FName, UGraph*>& InSaveData);
	UPROPERTY()
	TMap<FName,UGraph*> SavedGraphs;
};