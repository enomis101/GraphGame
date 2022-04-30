
#pragma once

#include "CoreMinimal.h"
#include <GameFramework/SaveGame.h>
#include "GraphSaveGame.generated.h"

class UGraph;


UCLASS(BlueprintType, Blueprintable)
class GRAPHGAME_API UGraphSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	//UPROPERTY(SaveGame)
	//TMap<FName, UGraph> SavedGraphs;

	UPROPERTY(SaveGame)
	int32 SaveVersion = 1;

	static constexpr int32 CurrentSaveVersion = 1;

	UPROPERTY(SaveGame)
	TArray<uint8> MemoryBuffer;
};