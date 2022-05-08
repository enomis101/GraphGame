#pragma once

#include "CoreMinimal.h"
#include "GraphFunctionLibrary.generated.h"

class UGraph;


UCLASS()
class GRAPHGAME_API UGraphFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	////
	//UFUNCTION(BlueprintCallable, Category = "Graph")
	//static ;
	UFUNCTION(BlueprintCallable, Category = "Graph")
		static bool IsGraphConnected(UGraph* G);
};