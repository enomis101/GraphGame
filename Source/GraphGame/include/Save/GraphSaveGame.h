
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
	UPROPERTY(SaveGame)
	TMap<FName, UGraph*> SavedGraphs;
};