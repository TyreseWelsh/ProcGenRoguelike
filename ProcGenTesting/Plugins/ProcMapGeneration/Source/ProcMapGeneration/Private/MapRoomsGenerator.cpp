// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoomsGenerator.h"
#include "MapGeneratorComponent.h"
#include "ExitGenerator.h"
#include "MapRoom.h"

void UMapRoomsGenerator::Init(int NewTileSize, FRoomData NewRoomData, UMapGeneratorComponent* NewMapGenerator)
{
	TileSize = NewTileSize;
	MapGenerator = NewMapGenerator;
	StartSplit(NewRoomData);
}

void UMapRoomsGenerator::StartSplit(FRoomData NewRoomData)
{
	if(NewRoomData.BSPSplitsRemaining > 0)
	{
		SplitRoom(NewRoomData);
	}
	else
	{
		// Can no longer split room
		SpawnRoom(NewRoomData);
	}	
}

void UMapRoomsGenerator::SplitRoom(FRoomData CurrentRoomData)
{
	//float SplitPercent = FMath::RandRange(0.35, 0.65);
	float num1 = 1 / (MapGenerator->InitialRoomSplitNum * 2);
	int num2 = FMath::RandRange(1, MapGenerator->InitialRoomSplitNum);
	if(num2 % 2 == 0)
	{
		num2 -= 1;
	}
	float SplitPercent = num1 * num2;
	
	if(CurrentRoomData.SizeY >= CurrentRoomData.SizeX)
	{
		// Check to split horizontally
		// Making sure both rooms produced by the split are larger than the minimum room size
		if(MinRoomSize < SplitPercent * CurrentRoomData.SizeY && MinRoomSize < (1 - SplitPercent) * CurrentRoomData.SizeY)
		{
			SplitHorizontally(SplitPercent, CurrentRoomData);
			return;
		}
		

		// Trying split again straight down the middle to be sure this room is too small to be split before we move on
		SplitPercent = 0.5f;
		if(MinRoomSize > (SplitPercent * CurrentRoomData.SizeY))
		{
			SpawnRoom(CurrentRoomData);
			return;
		}

		SplitHorizontally(SplitPercent, CurrentRoomData);
	}
	else
	{
		// Check to split vertically
		//
		if(MinRoomSize < SplitPercent * CurrentRoomData.SizeX && MinRoomSize < (1 - SplitPercent) * CurrentRoomData.SizeX)
		{
			SplitVertically(SplitPercent, CurrentRoomData);
			return;
		}

		//
		SplitPercent = 0.5f;
		if(MinRoomSize > (SplitPercent * CurrentRoomData.SizeX))
		{
			SpawnRoom(CurrentRoomData);
			return;
		}

		SplitVertically(SplitPercent, CurrentRoomData);
	}
}

void UMapRoomsGenerator::SplitHorizontally(const float SplitPercent, FRoomData CurrentRoomData)
{
	int RoomBottom = CurrentRoomData.Origin.Y + CurrentRoomData.SizeY;

	int SplitLocationY = CurrentRoomData.Origin.Y + ((RoomBottom - CurrentRoomData.Origin.Y) * SplitPercent);
	SplitLocationY = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationY, false) : RoundToTileSizeMultiple(SplitLocationY, true);
	
	// Setup and spawn of Exit actor along split
	int RandLocationAlongSplit;
	if(CurrentRoomData.BSPSplitsRemaining == 1)
	{
		RandLocationAlongSplit = CurrentRoomData.SizeX * FMath::RandRange(0.225, 0.775);
	}
	else
	{
		RandLocationAlongSplit = CurrentRoomData.SizeX *
			(FMath::RandBool() ? FMath::RandRange(LowerSplitPercentMin, LowerSplitPercentMax) : FMath::RandRange(UpperSplitPercentMin, UpperSplitPercentMax));
	}
	
	int ExitSpawnPosX = CurrentRoomData.Origin.X + RoundToTileSizeMultiple(RandLocationAlongSplit, true);

	FVector ExitSpawnPos = FVector(ExitSpawnPosX, SplitLocationY, CurrentRoomData.Origin.Z);
	SpawnExit(ExitSpawnPos, FRotator::ZeroRotator);
	
	
	// Creating and initialising child rooms
	FRoomData LeftRoomData(FVector(CurrentRoomData.Origin.X, CurrentRoomData.Origin.Y, CurrentRoomData.Origin.Z + RoomSpawnOffsetZ),
		CurrentRoomData.SizeX, CalculateLeftRoomSize(SplitLocationY, CurrentRoomData.Origin.Y), CurrentRoomData.BSPSplitsRemaining - 1,
		CurrentRoomData.MinPadding, CurrentRoomData.MaxPadding);
	StartSplit(LeftRoomData);

	int RightRoomOriginY = SplitLocationY + TileSize;
	FRoomData RightRoomData(FVector(CurrentRoomData.Origin.X, RightRoomOriginY, CurrentRoomData.Origin.Z + RoomSpawnOffsetZ),
		CurrentRoomData.SizeX, CalculateRightRoomSize(RightRoomOriginY, RoomBottom), CurrentRoomData.BSPSplitsRemaining - 1,
		CurrentRoomData.MinPadding, CurrentRoomData.MaxPadding);
	StartSplit(RightRoomData);
}

void UMapRoomsGenerator::SplitVertically(const float SplitPercent, FRoomData CurrentRoomData)
{
	int RoomRight = CurrentRoomData.Origin.X + CurrentRoomData.SizeX;
	
	int SplitLocationX = CurrentRoomData.Origin.X + ((RoomRight - CurrentRoomData.Origin.X) * SplitPercent);
	SplitLocationX = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationX, false) : RoundToTileSizeMultiple(SplitLocationX, true);
	
	int RandLocationAlongSplit;
	if(CurrentRoomData.BSPSplitsRemaining == 1)
	{
		RandLocationAlongSplit = CurrentRoomData.SizeY * FMath::RandRange(0.225, 0.775);
	}
	else
	{
		RandLocationAlongSplit = CurrentRoomData.SizeY *
			(FMath::RandBool() ? FMath::RandRange(LowerSplitPercentMin, LowerSplitPercentMax) : FMath::RandRange(UpperSplitPercentMin, UpperSplitPercentMax));
	}
	
	int ExitSpawnPosY = CurrentRoomData.Origin.Y + RoundToTileSizeMultiple(RandLocationAlongSplit, true);
	
	FVector ExitSpawnPos = FVector(SplitLocationX, ExitSpawnPosY, CurrentRoomData.Origin.Z);
	SpawnExit(ExitSpawnPos, FRotator(0, 90, 0));
	
	// Creating and initialising child rooms
	FRoomData LeftRoomData(FVector(CurrentRoomData.Origin.X, CurrentRoomData.Origin.Y, CurrentRoomData.Origin.Z + RoomSpawnOffsetZ),
	CalculateLeftRoomSize(SplitLocationX, CurrentRoomData.Origin.X), CurrentRoomData.SizeY, CurrentRoomData.BSPSplitsRemaining - 1,
	CurrentRoomData.MinPadding, CurrentRoomData.MaxPadding);
	
	StartSplit(LeftRoomData);

	
	int RightRoomOriginX = SplitLocationX + TileSize;
	FRoomData RightRoomData(FVector(RightRoomOriginX, CurrentRoomData.Origin.Y, CurrentRoomData.Origin.Z + RoomSpawnOffsetZ),
	CalculateRightRoomSize(RightRoomOriginX, RoomRight), CurrentRoomData.SizeY, CurrentRoomData.BSPSplitsRemaining - 1,
	CurrentRoomData.MinPadding, CurrentRoomData.MaxPadding);

	StartSplit(RightRoomData);
}

void UMapRoomsGenerator::SpawnExit(FVector SpawnPos, FRotator SpawnRot)
{
	TObjectPtr<AExitGenerator> Exit = MapGenerator->GetWorld()->SpawnActor<AExitGenerator>(AExitGenerator::StaticClass(), SpawnPos, SpawnRot);
	Exit->Init(TileSize);
	MapGenerator->AllRoomExits.Add(Exit);
}

void UMapRoomsGenerator::SpawnRoom(FRoomData RoomData)
{
	AMapRoom* NewRoom = MapGenerator->GetWorld()->SpawnActor<AMapRoom>(RoomClass, RoomData.Origin, FRotator::ZeroRotator);
	NewRoom->Init(&RoomData, MapGenerator);
	MapGenerator->AddToMapRooms(NewRoom);
}

float UMapRoomsGenerator::RoundToTileSizeMultiple(const float OldValue, bool const bRoundUp)
{
	float CurrentValue = OldValue / TileSize;
	CurrentValue = (bRoundUp) ? ceil(CurrentValue) : floor(CurrentValue);
	
	return CurrentValue * TileSize;
}

int UMapRoomsGenerator::CalculateLeftRoomSize(const int SplitLocation, const int Origin)
{
	return SplitLocation - Origin;
}

int UMapRoomsGenerator::CalculateRightRoomSize(const int SplitLocation, const int RoomEnd)
{
	return RoomEnd - SplitLocation;
}