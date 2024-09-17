// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MapGeneratorComponent.generated.h"

class UMapRoom;
class FastNoiseLite;
class AExitGenerator;

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
	
	// Helper functions
	float CalculateNodeXPos(int Index);
	float CalculateNodeYPos(int Index);
	float RoundToTileSizeMultiple(float OldValue, bool bRoundUp);
	FVector2D ConvertIndex1Dto2D(int index);
	int ConvertIndex2DTo1D(FVector2D Index2D);
	int CalculateMapIndexFromTilePos(FVector TilePos);

	float CalculateTileHeight(int x, int y);

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Map")
	TSubclassOf<AActor> TileClass;
	UPROPERTY()
	FVector MapOrigin = FVector::Zero();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	int MapSizeX = 3520;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	int MapSizeY = 3520;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	int DistBetweenNodes = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	int TileSize = 32;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Map")
	int InitialRoomSplitNum = 4;

	// Noise
	TSharedPtr<FastNoiseLite> HeightNoise;
	UPROPERTY()
	TArray<float> MapTileHeights;
	
	// Room properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Room")
	int RoomMinSize = 512;

	// Other properties
	UPROPERTY()
	TArray<UMapRoom*> LeafRooms;			
	UPROPERTY()
	TObjectPtr<UMapRoom> RootRoom;
	UPROPERTY()
	int LeafRoomIndex = 0;
	TArray<AExitGenerator*> AllRoomExits;
};
