// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapRoom.generated.h"

class UMapGeneratorComponent;
class UWorld;
class ABattleManager;
class ATBActor;

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
	int BSPSplitsRemaining = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MinPadding = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int MaxPadding = 0;
	

	bool bIsActive = false;
};


/**
 * Is not a physical object in the world.
 * Simply acts as a data object to store the room data such as Room: location, type, contained tiles, etc.
 */
UCLASS(Blueprintable)
class PROCMAPGENERATION_API AMapRoom : public AActor
{
	GENERATED_BODY()

public:
	void Init(FRoomData* NewRoomData, UMapGeneratorComponent* MapGeneratorComponent);

	void GenerateTiles();
	AActor* SpawnTile(FVector TileSpawnPos, int RoomIndexX = -1, int RoomIndexY = -1);

	void Enter();
	void Activate();
	void Leave();

	FRoomData* GetRoomData() { return RoomData; }
	TArray<AActor*> GetRoomTiles() { return RoomTiles; }
	TArray<ATBActor*> GetRoomObjects() { return RoomObjects; }

	// Debug Properties - All but "RoomColour" will be set on initialisation by the assigned MapGenerator
	bool bDebugEnabled = false;
	float RoomDrawDelay = 0.f;
	float RoomSpawnOffsetZ = 0.f;
	FLinearColor RoomColor = FLinearColor::Red;
	float TileSize = 32.f;
	FTimerHandle RoomSplitTimerHandle;
	FTimerDelegate RoomSplitTimerDelegate;
	
	// Main Properties
	int MinRoomSize = 0;

	float LowerSplitPercentMin = 0.2;
	float LowerSplitPercentMax = 0.3;
	float UpperSplitPercentMin = 0.7;
	float UpperSplitPercentMax = 0.8;
	
	int LastIndexX = 0;
	int LastIndexY = 0;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> RoomTiles;
	UPROPERTY(VisibleAnywhere)
	TArray<ATBActor*> RoomObjects;
	
	TObjectPtr<UMapGeneratorComponent> MapGenerator;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> TileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABattleManager> BattleManagerClass;
	TObjectPtr<ABattleManager> RoomBattleManager;


private:
	//
	FRoomData* RoomData;
};
