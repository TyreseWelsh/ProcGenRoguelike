// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapRoom.generated.h"

class UMapGeneratorComponent;
class UWorld;

/**
 * Is not a physical object in the world.
 * Simply acts as a data object to store the room data such as Room: location, type, contained tiles, etc.
 */
UCLASS()
class PROCMAPGENERATION_API UMapRoom : public UObject
{
	GENERATED_BODY()

public:
	UMapRoom();
	~UMapRoom();

	void InitRoom(UMapGeneratorComponent* NewMapGenerator, UMapRoom* ParentMapRoom, FVector Origin, int SizeX, int SizeY, int SplitsRemaining);
	UFUNCTION()
	void SplitRoom();
	void SplitHorizontally(const float SplitPercent);
	void SplitVertically(const float SplitPercent);
	void GenerateTiles();
	void SetTileType(int IndexX, int LastIndexX, int IndexY, int LastIndexY);
	TArray<UMapRoom*> GetLeaves(); 

	// Helper functions
	float RoundToTileSizeMultiple(const float OldValue, const bool bRoundUp);
	int CalculateLeftRoomSize(const int SplitLocation, const int Origin);
	int CalculateRightRoomSize(const int SplitLocation, const int RoomEnd);

	
	// Debug Properties - All but "RoomColour" will be set on initialisation by the assigned MapGenerator
	bool bDebugEnabled = false;
	float RoomDrawDelay = 0.f;
	float RoomSpawnOffsetZ = 0.f;
	FLinearColor RoomColor = FLinearColor::Red;
	float TileSize = 32.f;
	FTimerHandle RoomSplitTimerHandle;
	FTimerDelegate RoomSplitTimerDelegate;
	
	// Main Properties
	TObjectPtr<UMapGeneratorComponent> MapGenerator;
	FVector RoomOrigin = FVector::Zero();
	int RoomSizeX = 0;
	int RoomSizeY = 0;
	int MinRoomSize = 0;
	int NumSplitsRemaining = 0;
	
	TObjectPtr<UMapRoom> ParentRoom;
	TObjectPtr<UMapRoom> LeftChildRoom;
	TObjectPtr<UMapRoom> RightChildRoom;
	TArray<UMapRoom*> ChildLeafRooms;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> RoomTiles;

	int LastIndexX = 0;
	int LastIndexY = 0;
};
