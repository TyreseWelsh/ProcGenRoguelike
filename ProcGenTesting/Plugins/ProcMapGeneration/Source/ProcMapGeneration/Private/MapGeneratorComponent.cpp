// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorComponent.h"

#include "MapRoom.h"
#include "ExitGenerator.h"
#include "TileComponent.h"
#include "FastNoiseLite.h"


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
	AllRoomExits.Empty();
	MapTileHeights.Empty();

	int MapOriginX = RoundToTileSizeMultiple(GetOwner()->GetActorLocation().X, false);
	int MapOriginY = RoundToTileSizeMultiple(GetOwner()->GetActorLocation().Y, false);
	MapOrigin =  FVector(MapOriginX, MapOriginY, GetOwner()->GetActorLocation().Z);
	
	MapSizeX = RoundToTileSizeMultiple(MapSizeX, false);
	MapSizeY = RoundToTileSizeMultiple(MapSizeY, false);
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
		RootRoom->InitRoom(this, nullptr, MapOrigin, MapSizeX, MapSizeY, InitialRoomSplitNum);
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
			if(IsValid(MapTiles[CalculateMapIndexFromTilePos(Exit->LeftExitTilePos)]))
			{
				MapTiles[CalculateMapIndexFromTilePos(Exit->LeftExitTilePos)]->FindComponentByClass<UTileComponent>()->SetTileTypeToExit();
			}
			if(IsValid(MapTiles[CalculateMapIndexFromTilePos(Exit->RightExitTilePos)]))
			{
				MapTiles[CalculateMapIndexFromTilePos(Exit->RightExitTilePos)]->FindComponentByClass<UTileComponent>()->SetTileTypeToExit();
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

float UMapGeneratorComponent::RoundToTileSizeMultiple(float OldValue, bool bRoundUp)
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
}

float UMapGeneratorComponent::CalculateTileHeight(int x, int y)
{
	int MapIndex1D = CalculateMapIndexFromTilePos(FVector(x, y, 0));
	FVector2D MapIndex2D = ConvertIndex1Dto2D(MapIndex1D);
	
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

