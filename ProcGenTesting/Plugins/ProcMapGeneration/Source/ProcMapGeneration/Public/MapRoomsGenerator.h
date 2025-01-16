// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapRoomsGenerator.generated.h"


class UMapGeneratorComponent;
class AMapRoom;

struct FRoomData;


/**
 * 
 */
UCLASS(Blueprintable)
class PROCMAPGENERATION_API UMapRoomsGenerator : public UObject
{
	GENERATED_BODY()

public:
	void Init(int NewTileSize, FRoomData NewRoomData, UMapGeneratorComponent* NewMapGenerator);
	void StartSplit(FRoomData NewRoomData);
	UFUNCTION()
	void SplitRoom(FRoomData CurrentRoomData);
	void SplitHorizontally(const float SplitPercent, FRoomData CurrentRoomData);
	void SplitVertically(const float SplitPercent, FRoomData CurrentRoomData);

	void SpawnExit(FVector SpawnPos, FRotator SpawnRot);
	void SpawnRoom(FRoomData RoomData);

	float RoundToTileSizeMultiple(const float OldValue, const bool bRoundUp);
	int CalculateLeftRoomSize(const int SplitLocation, const int Origin);
	int CalculateRightRoomSize(const int SplitLocation, const int RoomEnd);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	int MinRoomSize = 0;
	float TileSize = 32.f;

	float LowerSplitPercentMin = 0.2;
	float LowerSplitPercentMax = 0.3;
	float UpperSplitPercentMin = 0.7;
	float UpperSplitPercentMax = 0.8;

	// Debug properties
	float RoomSpawnOffsetZ = 0.f;


	TObjectPtr<UMapGeneratorComponent> MapGenerator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	TSubclassOf<AMapRoom> RoomClass;
};
