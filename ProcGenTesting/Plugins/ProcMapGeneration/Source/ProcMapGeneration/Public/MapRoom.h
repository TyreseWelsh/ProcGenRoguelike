// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapRoom.generated.h"

class UMapGeneratorComponent;
class UWorld;

USTRUCT(BlueprintType)
struct FRoomData
{
	GENERATED_BODY()

	FRoomData() {}
	FRoomData(FVector RoomOrigin, int RoomSizeX, int RoomSizeY, int RoomSplitsRemaining, int RoomMinPadding, int RoomMaxPadding)
		: Origin(RoomOrigin), SizeX(RoomSizeX), SizeY(RoomSizeY), BSPSplitsRemaining(RoomSplitsRemaining), MinPadding(RoomMinPadding), MaxPadding(RoomMaxPadding)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Origin = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SizeX = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int SizeY = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MinPadding = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxPadding = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int BSPSplitsRemaining = 0;

	bool bIsActive = false;
};

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

	void InitRoom(UMapGeneratorComponent* NewMapGenerator, UMapRoom* ParentMapRoom, FRoomData NewRoomData);
	UFUNCTION()
	void SplitRoom();
	void SplitHorizontally(const float SplitPercent);
	void SplitVertically(const float SplitPercent);
	void GenerateTiles();
	AActor* SpawnTile(FVector TileSpawnPos, int RoomIndexX = -1, int RoomIndexY = -1);
	void ChooseTileType(FVector TilePos);
	TArray<UMapRoom*> GetLeaves(); 

	// Helper functions
	float RoundToTileSizeMultiple(const float OldValue, const bool bRoundUp);
	int CalculateLeftRoomSize(const int SplitLocation, const int Origin);
	int CalculateRightRoomSize(const int SplitLocation, const int RoomEnd);

	FRoomData GetRoomData() { return RoomData; }
	
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
	/*FVector RoomOrigin = FVector::Zero();
	int RoomSizeX = 0;
	int RoomSizeY = 0;
	int NumSplitsRemaining = 0;*/
	
	int MinRoomSize = 0;

	TObjectPtr<UMapRoom> ParentRoom;
	TObjectPtr<UMapRoom> LeftChildRoom;
	TObjectPtr<UMapRoom> RightChildRoom;
	TArray<UMapRoom*> ChildLeafRooms;
	
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> RoomTiles;

	float LowerMin = 0.2;
	float LowerMax = 0.3;
	float UpperMin = 0.7;
	float UpperMax = 0.8;
	
	int LastIndexX = 0;
	int LastIndexY = 0;

private:
	//
	FRoomData RoomData;
};
