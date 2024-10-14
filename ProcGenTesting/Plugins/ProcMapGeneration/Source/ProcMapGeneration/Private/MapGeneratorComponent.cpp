// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorComponent.h"

#include "MapRoom.h"
#include "ExitGenerator.h"
#include "TileComponent.h"
#include "IsTile.h"
#include "FastNoiseLite.h"
#include "TileMapFunctionLibrary.h"


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
		Exit->GetLeftTeleportPoint()->Destroy();
		Exit->GetRightTeleportPoint()->Destroy();
		Exit->Destroy();
	}
	AllRoomExits.Empty();
	MapTileHeights.Empty();

	int MapOriginX = UTileMapFunctionLibrary::RoundToTileSizeMultiple(GetOwner()->GetActorLocation().X, false, TileSize);
	int MapOriginY = UTileMapFunctionLibrary::RoundToTileSizeMultiple(GetOwner()->GetActorLocation().Y, false, TileSize);
	MapOrigin =  FVector(MapOriginX, MapOriginY, GetOwner()->GetActorLocation().Z);
	
	MapSizeX = UTileMapFunctionLibrary::RoundToTileSizeMultiple(MapSizeX, false, TileSize);
	MapSizeY = UTileMapFunctionLibrary::RoundToTileSizeMultiple(MapSizeY, false, TileSize);
	int NumTilesX = MapSizeX / TileSize;
	int NumTilesY = MapSizeY / TileSize;
	
	MapTiles.SetNum(NumTilesX * NumTilesY);

	int seed = FMath::RandRange(1338, 999999);
	HeightNoise = MakeShared<FastNoiseLite>(seed);
	HeightNoise->SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	float freq = FMath::RandRange(0.04, 0.0575);
	HeightNoise->SetSeed(seed);
	HeightNoise->SetFrequency(freq);

	HeightNoise->SetFractalType(FastNoiseLite::FractalType_DomainWarpProgressive);
	
	UE_LOG(LogTemp, Warning, TEXT("Seed is is : %i"), seed);
	UE_LOG(LogTemp, Warning, TEXT("Frequency is : %f"), freq);

	MapTileHeights.SetNum(NumTilesX * NumTilesY);

	
	RootRoom = NewObject<UMapRoom>();
	if(IsValid(RootRoom))
	{
		FRoomData InitialRoomData(MapOrigin, MapSizeX, MapSizeY, InitialRoomSplitNum, RoomMinPadding, RoomMaxPadding);
		RootRoom->InitRoom(this, nullptr, InitialRoomData);
	}

	/*for(AActor* Tile : MapTiles)
	{
		if(UTileComponent* TComponent = Tile->FindComponentByClass<UTileComponent>())
		{
			TComponent->CheckSurroundedByWalls();
		}
	}*/
	
	// Set room exit tiles
	if(AllRoomExits.Num() > 0)
	{
		for(AExitGenerator* Exit : AllRoomExits)
		{
			if(IsValid(Exit->GetLeftTeleportPoint()))
			{
				UTileMapFunctionLibrary::OccupyTile(Exit->GetLeftTeleportPoint());
			}

			if(IsValid(Exit->GetRightTeleportPoint()))
			{
				UTileMapFunctionLibrary::OccupyTile(Exit->GetRightTeleportPoint());
			}
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

/*float UMapGeneratorComponent::RoundToTileSizeMultiple(float OldValue, bool bRoundUp)
{
	float CurrentValue = OldValue / TileSize;
	CurrentValue = (bRoundUp) ? ceil(CurrentValue) : floor(CurrentValue);
	
	return CurrentValue * TileSize;
}

FVector2D UMapGeneratorComponent::ConvertIndex1Dto2D(int index)
{
	int DivisionValue = index / (MapSizeX / TileSize);
	int Remainder = index % (MapSizeX / TileSize);
	
	return FVector2D(Remainder, DivisionValue);
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
}*/

float UMapGeneratorComponent::CalculateTileHeight(int x, int y)
{
	int MapIndex1D = UTileMapFunctionLibrary::CalculateIndexFromTilePos(FVector(x, y, 0), RootRoom->GetRoomData().Origin, RootRoom->GetRoomData().SizeX, TileSize);
	FVector2D MapIndex2D = UTileMapFunctionLibrary::ConvertIndex1Dto2D(MapIndex1D, RootRoom->GetRoomData().SizeX, TileSize);
	
	return HeightNoise->GetNoise(MapIndex2D.X, MapIndex2D.Y);
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

