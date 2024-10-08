// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoom.h"

#include "MapGeneratorComponent.h"
#include "Engine/World.h"
#include "TileComponent.h"
#include "ExitGenerator.h"

#include "DrawDebugHelpers.h"

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
	//float SplitPercent = FMath::RandRange(0.35, 0.65);
	float num1 = 1 / (MapGenerator->InitialRoomSplitNum * 2);
	int num2 = FMath::RandRange(1, MapGenerator->InitialRoomSplitNum);
	if(num2 % 2 == 0)
	{
		num2 -= 1;
	}
	float SplitPercent = num1 * num2;
	
	if(RoomSizeY >= RoomSizeX)
	{
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
	
	FColor NewRoomColor = 	FColor::MakeRandomColor();
	//
	for(int i = 0; i < (RoomSizeX / TileSize); ++i)
	{
		FVector CurrentTilePos = FVector(RoomOrigin.X + (i * TileSize), SplitLocationY, RoomOrigin.Z);
		if(AActor* CurrentTile = SpawnTile(CurrentTilePos))
		{
			if(UTileComponent* CurrenTileComponent = CurrentTile->FindComponentByClass<UTileComponent>())
			{
				CurrenTileComponent->SetTileTypeToWall();
			}
		}
	}
	//
	
	// Setup and spawn of Exit actor along split
	int RandLocationAlongSplit;
	if(NumSplitsRemaining == 1)
	{
		RandLocationAlongSplit = RoomSizeX * FMath::RandRange(0.225, 0.775);
	}
	else
	{
		RandLocationAlongSplit = RoomSizeX *
			(FMath::RandBool() ? FMath::RandRange(LowerMin, LowerMax) : FMath::RandRange(UpperMin, UpperMax));
	}
	
	int ExitSpawnPosX = RoomOrigin.X + RoundToTileSizeMultiple(RandLocationAlongSplit, true);

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
		int RightRoomOriginY = SplitLocationY + TileSize;
		RightChildRoom->InitRoom(MapGenerator, this, FVector(RoomOrigin.X, RightRoomOriginY, RoomOrigin.Z + RoomSpawnOffsetZ),
			RoomSizeX, CalculateRightRoomSize(RightRoomOriginY, RoomBottom), NumSplitsRemaining - 1);
	}
}

void UMapRoom::SplitVertically(const float SplitPercent)
{
	int RoomRight = RoomOrigin.X + RoomSizeX;
	
	int SplitLocationX = RoomOrigin.X + ((RoomRight - RoomOrigin.X) * SplitPercent);
	SplitLocationX = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationX, false) : RoundToTileSizeMultiple(SplitLocationX, true);
	
	FColor NewRoomColor = 	FColor::MakeRandomColor();
	//
	for(int i = 0; i < (RoomSizeY / TileSize); ++i)
	{
		FVector CurrentTilePos = FVector(SplitLocationX, RoomOrigin.Y + (i * TileSize), RoomOrigin.Z);

		if(AActor* CurrentTile = SpawnTile(CurrentTilePos))
		{
			if(UTileComponent* CurrenTileComponent = CurrentTile->FindComponentByClass<UTileComponent>())
			{
				CurrenTileComponent->SetTileTypeToWall();
			}
		}
	}
	//
	UE_LOG(LogTemp, Warning, TEXT(""));
	int RandLocationAlongSplit;
	if(NumSplitsRemaining == 1)
	{
		RandLocationAlongSplit = RoomSizeY * FMath::RandRange(0.225, 0.775);
	}
	else
	{
		RandLocationAlongSplit = RoomSizeY *
			(FMath::RandBool() ? FMath::RandRange(LowerMin, LowerMax) : FMath::RandRange(UpperMin, UpperMax));
	}
	
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
		int RightRoomOriginX = SplitLocationX + TileSize;
		RightChildRoom->InitRoom(MapGenerator, this, FVector(RightRoomOriginX, RoomOrigin.Y, RoomOrigin.Z + RoomSpawnOffsetZ),
		CalculateRightRoomSize(RightRoomOriginX, RoomRight), RoomSizeY, NumSplitsRemaining - 1);
	}
}

void UMapRoom::GenerateTiles()
{
	FVector RoomStartPos = RoomOrigin;
	FVector RoomEndPos = FVector(RoomStartPos.X + RoomSizeX, RoomStartPos.Y + RoomSizeY, RoomStartPos.Z);
	
	LastIndexX = ((RoomEndPos.X - RoomStartPos.X) / TileSize) - 1;
	for(int i = 0; i <= LastIndexX; ++i)
	{
		LastIndexY = ((RoomEndPos.Y - RoomStartPos.Y) / TileSize) - 1;
		for (int j = 0; j <= LastIndexY; ++j)
		{
			FVector TileSpawnPos = FVector::ZeroVector;
			TileSpawnPos.X = RoomStartPos.X + i * TileSize;
			TileSpawnPos.Y = RoomStartPos.Y + j * TileSize;

			RoomTiles.Add(SpawnTile(TileSpawnPos, i, j));
		}
	}
	
	RoomSplitTimerHandle.Invalidate();
}

AActor* UMapRoom::SpawnTile(FVector TileSpawnPos, int RoomIndexX, int RoomIndexY)
{
	int TileMapArrayIndex = MapGenerator->CalculateMapIndexFromTilePos(TileSpawnPos);
	TObjectPtr<AActor> CurrentTile;
	if(TileMapArrayIndex >= MapGenerator->MapTiles.Num())
	{
		//DrawDebugLine(MapGenerator->GetOwner()->GetWorld(), TileSpawnPos, FVector(TileSpawnPos.X, TileSpawnPos.Y, TileSpawnPos.Z + 1000.f), FColor::Red, false, 6000, 0, 10.f);
	}
	else
	{
		// Spawns new tile actor if it does not exist inside the "MapTiles" array
		if(!IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
		{
			if(IsValid(MapGenerator->GetWorld()))
			{
				CurrentTile = MapGenerator->GetWorld()->SpawnActor<AActor>(MapGenerator->TileClass, TileSpawnPos, FRotator::ZeroRotator);
				MapGenerator->MapTiles[TileMapArrayIndex] = CurrentTile;
			}
		}
			
		// Initialises the current tile component
		if(IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
		{
			CurrentTile = MapGenerator->MapTiles[TileMapArrayIndex];
			CurrentTile->SetActorLocation(TileSpawnPos);
			if(UTileComponent* CurrentTileComponent = CurrentTile->FindComponentByClass<UTileComponent>())
			{
				CurrentTileComponent->InitTile(this, TileSize, RoomIndexX, RoomIndexY);
			}
		}
	}
	
	return CurrentTile;
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
