// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapGeneratorComponent.generated.h"

class AMapRoom;
class FastNoiseLite;
class AExitGenerator;
struct FRoomData;
class UMapRoomsGenerator;
class URoomContentsManager;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCMAPGENERATION_API UMapGeneratorComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UMapGeneratorComponent();

	// Main functions
	UFUNCTION(BlueprintCallable)
	void InitMap();
	void SpawnRoom();
	
	// Helper functions
	float CalculateNodeXPos(int Index);
	float CalculateNodeYPos(int Index);

	float CalculateTileHeight(int x, int y);

	void AddToMapRooms(AMapRoom* NewRoom);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	// Debug properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDebugEnabled = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float RoomSpawnOffsetZ = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float DebugDrawDelay = 1.5f;									// If bDebugEnabled == true, will delay spawning off room linetrace visuals
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float RoomSpawnTime = 0.0001f;									// Time between each rooms construction

	// Map properties
	UPROPERTY(BlueprintReadOnly, Category = "Map")
	TArray<AActor*> MapTiles;										// WIP: Will be changed to store pointers to tile node class
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<UMapRoomsGenerator> MapRoomsGeneratorClass;
	UPROPERTY()
	TObjectPtr<UMapRoomsGenerator> MapRoomsGenerator;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	TSubclassOf<URoomContentsManager> RoomContentsManagerClass;
	UPROPERTY()
	TObjectPtr<URoomContentsManager> RoomContentsManager;
	
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	FRoomData* InitialRoomData;*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initial Room Data")
	FVector MapOrigin = FVector::Zero();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initial Room Data")
	int MapSizeX = 3520;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initial Room Data")
	int MapSizeY = 3520;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initial Room Data")
	int DistBetweenNodes = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initial Room Data")
	int TileSize = 32;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Initial Room Data")
	int InitialRoomSplitNum = 6;

	// Noise
	TSharedPtr<FastNoiseLite> HeightNoise;
	UPROPERTY()
	TArray<float> MapTileHeights;
	
	// Room properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int RoomMinSize = 320;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int RoomMinPadding = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int RoomMaxPadding = 0;

	// Other properties
	UPROPERTY()
	TArray<AMapRoom*> MapRooms;			
	UPROPERTY()
	TObjectPtr<AMapRoom> RootRoom;
	UPROPERTY()
	int LeafRoomIndex = 0;
	TArray<AExitGenerator*> AllRoomExits;
};
