// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoom.h"

#include "MapGeneratorComponent.h"
#include "Engine/World.h"
#include "TileComponent.h"
#include "ExitGenerator.h"

UMapRoom::UMapRoom()
{
}

UMapRoom::~UMapRoom()
{
}

void UMapRoom::InitRoom(UMapGeneratorComponent* NewMapGenerator, UMapRoom* ParentMapRoom, FVector Origin, int SizeX, int SizeY, int SplitsRemaining)
{
	MapGenerator = NewMapGenerator;
	ParentRoom = ParentMapRoom;
	RoomOrigin = Origin;
	RoomSizeX = SizeX;
	RoomSizeY = SizeY;
	NumSplitsRemaining = SplitsRemaining;
	
	TileSize = MapGenerator->TileSize;
	MinRoomSize = MapGenerator->RoomMinSize;
	
	RoomSplitTimerDelegate.BindUFunction(this, "SplitRoom");

	if(IsValid(MapGenerator))
	{
		if(NumSplitsRemaining > 0)
		{
			if(bDebugEnabled)
			{
				// Only really does anything if I add a function to actually draw the room at some point during the split (I havent)
				MapGenerator->GetWorld()->GetTimerManager().SetTimer(RoomSplitTimerHandle, RoomSplitTimerDelegate, 0.0001f, false, RoomDrawDelay);
			}
			else
			{
				SplitRoom();
			}
		}
		else
		{
			// Can no longer split
			GenerateTiles();
		}	
	}
}

void UMapRoom::SplitRoom()
{
	float SplitPercent = FMath::RandRange(0.3, 0.7);


	if(RoomSizeY > RoomSizeX)
	{
		int aaa = (SplitPercent * RoomSizeY);
		// Check to split horizontally
		// Making sure both rooms produced by the split are larger than the minimum room size
		if(MinRoomSize < SplitPercent * RoomSizeY && MinRoomSize < (1 - SplitPercent) * RoomSizeY)
		{
			SplitHorizontally(SplitPercent);
			return;
		}
		

		// Trying split again straight down the middle to be sure this room is too small to be split before we move on
		SplitPercent = 0.5f;
		if(MinRoomSize > (SplitPercent * RoomSizeY))
		{
			GenerateTiles();
			return;
		}

		SplitHorizontally(SplitPercent);
	}
	else
	{
		// Check to split vertically
		//
		if(MinRoomSize < SplitPercent * RoomSizeX && MinRoomSize < (1 - SplitPercent) * RoomSizeX)
		{
			SplitVertically(SplitPercent);
			return;
		}

		//
		SplitPercent = 0.5f;
		if(MinRoomSize > (SplitPercent * RoomSizeX))
		{
			GenerateTiles();
			return;
		}

		SplitVertically(SplitPercent);
	}
}

void UMapRoom::SplitHorizontally(const float SplitPercent)
{
	int RoomBottom = RoomOrigin.Y + RoomSizeY;

	int SplitLocationY = RoomOrigin.Y + ((RoomBottom - RoomOrigin.Y) * SplitPercent);
	SplitLocationY = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationY, false) : RoundToTileSizeMultiple(SplitLocationY, true);
	
	// Setup and spawn of Exit actor along split
	int RandLocationAlongSplit = RoomSizeX * /*FMath::RandRange(0.25, 0.75);*/
		(FMath::RandBool() ? FMath::RandRange(0.1, 0.25) : FMath::RandRange(0.75, 0.9));
	int ExitSpawnPosX = RoomOrigin.X + RoundToTileSizeMultiple(RandLocationAlongSplit, true);
;
	FVector ExitSpawnPos = FVector(ExitSpawnPosX, SplitLocationY, RoomOrigin.Z);
	TObjectPtr<AExitGenerator> Exit = MapGenerator->GetWorld()->SpawnActor<AExitGenerator>(AExitGenerator::StaticClass(), ExitSpawnPos, FRotator(0, 0, 0));
	Exit->Init(TileSize);
	MapGenerator->AllRoomExits.Add(Exit);

	// Creating and initialising child rooms
	LeftChildRoom = NewObject<UMapRoom>();
	if(IsValid(LeftChildRoom))
	{
		LeftChildRoom->InitRoom(MapGenerator, this, FVector(RoomOrigin.X, RoomOrigin.Y, RoomOrigin.Z + RoomSpawnOffsetZ),
			RoomSizeX, CalculateLeftRoomSize(SplitLocationY, RoomOrigin.Y), NumSplitsRemaining - 1);
	}

	RightChildRoom = NewObject<UMapRoom>();
	if(IsValid(RightChildRoom))
	{
		RightChildRoom->InitRoom(MapGenerator, this, FVector(RoomOrigin.X, SplitLocationY, RoomOrigin.Z + RoomSpawnOffsetZ),
			RoomSizeX, CalculateRightRoomSize(SplitLocationY, RoomBottom), NumSplitsRemaining - 1);
	}
}

void UMapRoom::SplitVertically(const float SplitPercent)
{
	int RoomRight = RoomOrigin.X + RoomSizeX;
	
	int SplitLocationX = RoomOrigin.X + ((RoomRight - RoomOrigin.X) * SplitPercent);
	SplitLocationX = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationX, false) : RoundToTileSizeMultiple(SplitLocationX, true);
	
	// Setup and spawn of Exit actor along split
	int RandLocationAlongSplit = RoomSizeY *
		(FMath::RandBool() ? FMath::RandRange(0.1, 0.2) : FMath::RandRange(0.8, 0.9));
	int ExitSpawnPosY = RoomOrigin.Y + RoundToTileSizeMultiple(RandLocationAlongSplit, true);
	
	FVector ExitSpawnPos = FVector(SplitLocationX, ExitSpawnPosY, RoomOrigin.Z);
	TObjectPtr<AExitGenerator> Exit = MapGenerator->GetWorld()->SpawnActor<AExitGenerator>(AExitGenerator::StaticClass(), ExitSpawnPos, FRotator(0, 90, 0));
	Exit->Init(TileSize);
	MapGenerator->AllRoomExits.Add(Exit);

	// Creating and initialising child rooms
	LeftChildRoom = NewObject<UMapRoom>();
	if(IsValid(LeftChildRoom))
	{
		LeftChildRoom->InitRoom(MapGenerator, this, FVector(RoomOrigin.X, RoomOrigin.Y, RoomOrigin.Z + RoomSpawnOffsetZ),
		CalculateLeftRoomSize(SplitLocationX, RoomOrigin.X), RoomSizeY, NumSplitsRemaining - 1);
	}
	
	RightChildRoom = NewObject<UMapRoom>();
	if(IsValid(RightChildRoom))
	{
		RightChildRoom->InitRoom(MapGenerator, this, FVector(SplitLocationX, RoomOrigin.Y, RoomOrigin.Z + RoomSpawnOffsetZ),
		CalculateRightRoomSize(SplitLocationX, RoomRight), RoomSizeY, NumSplitsRemaining - 1);
	}
}

void UMapRoom::GenerateTiles()
{
	FVector RoomStartPos = RoomOrigin;
	FVector RoomEndPos = FVector(RoomStartPos.X + RoomSizeX, RoomStartPos.Y + RoomSizeY, RoomStartPos.Z);

	/*
	float RandZ = FMath::RandRange(-1000, 1000);
	*/
	
	LastIndexX = ((RoomEndPos.X - RoomStartPos.X) / TileSize) - 1;
	for(int i = 0; i <= LastIndexX; ++i)
	{
		LastIndexY = ((RoomEndPos.Y - RoomStartPos.Y) / TileSize) - 1;
		for (int j = 0; j <= LastIndexY; ++j)
		{
			FVector TileSpawnPos = FVector::ZeroVector;
			TileSpawnPos.X = RoomStartPos.X + i * TileSize;
			TileSpawnPos.Y = RoomStartPos.Y + j * TileSize;
			TileSpawnPos.Z = RoomStartPos.Z /*+ RandZ*/;

			// TODO: Call tile function to choose "TileType"
			
			int TileMapArrayIndex = MapGenerator->CalculateMapIndexFromTilePos(TileSpawnPos);

			// Spawns new tile actor if it does not exist inside the "MapTiles" array
			if(!IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
			{
				if(IsValid(MapGenerator->GetWorld()))
				{
					TObjectPtr<AActor> CurrentTile = MapGenerator->GetWorld()->SpawnActor<AActor>(MapGenerator->TileClass, TileSpawnPos, FRotator::ZeroRotator);
					MapGenerator->MapTiles[TileMapArrayIndex] = CurrentTile;
				}
			}
			
			// Initialises the current tile component
			if(IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
			{
				if(UTileComponent* CurrentTileComponent = MapGenerator->MapTiles[TileMapArrayIndex]->FindComponentByClass<UTileComponent>())
				{
					CurrentTileComponent->InitTile(this, i, j);
				}
				RoomTiles.Add(MapGenerator->MapTiles[TileMapArrayIndex]);
			}
		}
	}
	
	RoomSplitTimerHandle.Invalidate();
}



TArray<UMapRoom*> UMapRoom::GetLeaves()
{
	TArray<UMapRoom*> Leaves;
	if(!(IsValid(LeftChildRoom) && IsValid(RightChildRoom)))
	{
		Leaves.Add(this);
		return Leaves;
	}

	ChildLeafRooms.Append(LeftChildRoom->GetLeaves());
	ChildLeafRooms.Append(RightChildRoom->GetLeaves());
	return ChildLeafRooms;
}

float UMapRoom::RoundToTileSizeMultiple(const float OldValue, bool const bRoundUp)
{
	float CurrentValue = OldValue / TileSize;
	CurrentValue = (bRoundUp) ? ceil(CurrentValue) : floor(CurrentValue);
	
	return CurrentValue * TileSize;
}

int UMapRoom::CalculateLeftRoomSize(const int SplitLocation, const int Origin)
{
	return SplitLocation - Origin;
}

int UMapRoom::CalculateRightRoomSize(const int SplitLocation, const int RoomEnd)
{
	return RoomEnd - SplitLocation;
}
