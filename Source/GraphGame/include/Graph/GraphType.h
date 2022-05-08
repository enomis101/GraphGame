#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAlgorithmType : uint8 {
	BFS = 0 UMETA(DisplayName = "BFS"),
	DFS UMETA(DisplayName = "DFS"),
	DFSRecursive UMETA(DisplayName = "DFSRecursive"),
	Dijkstra UMETA(DisplayName = "Dijkstra"),
	MST_Kruskal UMETA(DisplayName = "Kruskal"),
	NA UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EAlgorithmType, EAlgorithmType::NA);