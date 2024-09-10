// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorComponent.h"

#include "MapRoom.h"
#include "ExitGenerator.h"
#include "TileComponent.h"


// Sets default values for this component's properties
UMapGeneratorComponent::UMapGeneratorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UMapGeneratorComponent::InitMap()
{
	// Clearing old objects
	RootRoom = nullptr;
	for(AExitGenerator* Exit : AllRoomExits)
	{
		Exit->Destroy();
	}
	
	MapSizeX = RoundToTileSizeMultiple(MapSizeX, false);
	MapSizeY = RoundToTileSizeMultiple(MapSizeY, false);
	MapTiles.SetNum((MapSizeX / TileSize) * (MapSizeY / TileSize));  

	RootRoom = NewObject<UMapRoom>();
	if(IsValid(RootRoom))
	{
		RootRoom->InitRoom(this, nullptr, GetOwner()->GetActorLocation(), MapSizeX, MapSizeY, InitialRoomSplitNum);
	}

	if(AllRoomExits.Num() > 0)
	{
		for(AExitGenerator* Exit : AllRoomExits)
		{
			// TODO: Will need to create blueprint class of Exit actor that will be spawned and stored in this array
			// Get Left and right tile vectors from Exit and use to calculate map index.
			if(IsValid(MapTiles[CalculateMapIndexFromTilePos(Exit->LeftExitTilePos)]))
			{
				MapTiles[CalculateMapIndexFromTilePos(Exit->LeftExitTilePos)]->FindComponentByClass<UTileComponent>()->SetTileTypeToExit();
			}
			if(IsValid(MapTiles[CalculateMapIndexFromTilePos(Exit->RightExitTilePos)]))
			{
				MapTiles[CalculateMapIndexFromTilePos(Exit->RightExitTilePos)]->FindComponentByClass<UTileComponent>()->SetTileTypeToExit();
			}
			// If valid tiles are found, SetTileTypeExit
		}
	}
}

float UMapGeneratorComponent::CalculateNodeXPos(int Index)
{
	return DistBetweenNodes * (Index % MapSizeX);
}

float UMapGeneratorComponent::CalculateNodeYPos(int Index)
{
	return DistBetweenNodes * (Index / MapSizeX);
}

float UMapGeneratorComponent::RoundToTileSizeMultiple(float OldValue, bool bRoundUp)
{
	float CurrentValue = OldValue / TileSize;
	CurrentValue = (bRoundUp) ? ceil(CurrentValue) : floor(CurrentValue);
	
	return CurrentValue * TileSize;
}

int UMapGeneratorComponent::ConvertIndex2DTo1D(FVector2D Index2D)
{
	return Index2D.X + (Index2D.Y * (MapSizeX / TileSize));
}

int UMapGeneratorComponent::CalculateMapIndexFromTilePos(FVector TilePos)
{
	int x = (TilePos.X - MapOrigin.X) / TileSize;
	int y = (TilePos.Y - MapOrigin.Y) / TileSize;

	return ConvertIndex2DTo1D(FVector2D(x, y));
}


// Called when the game starts
void UMapGeneratorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMapGeneratorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

