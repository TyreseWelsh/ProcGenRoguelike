// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TileMapFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROCMAPGENERATION_API UTileMapFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static float RoundToTileSizeMultiple(float OldValue, bool bRoundUp, int TileSize);

	UFUNCTION(BlueprintCallable)
	static int CalculateIndexFromTilePos(FVector TilePos, FVector AreaOrigin, int AreaSizeX, int TileSize);

	UFUNCTION(BlueprintCallable)
	static FVector2D ConvertIndex1Dto2D(int index, int SizeX, int TileSize);

	UFUNCTION(BlueprintCallable)
	static int ConvertIndex2DTo1D(FVector2D Index2D, int SizeX, int TileSize);


	UFUNCTION(BlueprintCallable)
	static AActor* GetBelowTile(AActor* StartingActor);
	static AActor* GetBelowTile(FVector StartingPos, UWorld* CurrentWorld);
	UFUNCTION(BlueprintCallable)
	static bool OccupyTile(AActor* OccupyingActor);
	UFUNCTION(BlueprintCallable)
	static bool UnOccupyTile(AActor* UnOccupyingActor);
};
