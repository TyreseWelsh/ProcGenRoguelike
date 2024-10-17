// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoom.h"

#include "MapGeneratorComponent.h"
#include "Engine/World.h"
#include "TileComponent.h"
#include "IsTile.h"
#include "ExitGenerator.h"
#include "TileMapFunctionLibrary.h"

#include "DrawDebugHelpers.h"

void UMapRoom::InitRoom(UMapGeneratorComponent* NewMapGenerator, UMapRoom* ParentMapRoom, FRoomData NewRoomData)
{
	MapGenerator = NewMapGenerator;
	ParentRoom = ParentMapRoom;

	RoomData = NewRoomData;
	
	TileSize = MapGenerator->TileSize;
	MinRoomSize = MapGenerator->RoomMinSize;
	
	RoomSplitTimerDelegate.BindUFunction(this, "SplitRoom");

	if(IsValid(MapGenerator))
	{
		if(RoomData.BSPSplitsRemaining > 0)
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
			// Can no longer split room
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
	
	if(RoomData.SizeY >= RoomData.SizeX)
	{
		// Check to split horizontally
		// Making sure both rooms produced by the split are larger than the minimum room size
		if(MinRoomSize < SplitPercent * RoomData.SizeY && MinRoomSize < (1 - SplitPercent) * RoomData.SizeY)
		{
			SplitHorizontally(SplitPercent);
			return;
		}
		

		// Trying split again straight down the middle to be sure this room is too small to be split before we move on
		SplitPercent = 0.5f;
		if(MinRoomSize > (SplitPercent * RoomData.SizeY))
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
		if(MinRoomSize < SplitPercent * RoomData.SizeX && MinRoomSize < (1 - SplitPercent) * RoomData.SizeX)
		{
			SplitVertically(SplitPercent);
			return;
		}

		//
		SplitPercent = 0.5f;
		if(MinRoomSize > (SplitPercent * RoomData.SizeX))
		{
			GenerateTiles();
			return;
		}

		SplitVertically(SplitPercent);
	}
}

void UMapRoom::SplitHorizontally(const float SplitPercent)
{
	int RoomBottom = RoomData.Origin.Y + RoomData.SizeY;

	int SplitLocationY = RoomData.Origin.Y + ((RoomBottom - RoomData.Origin.Y) * SplitPercent);
	SplitLocationY = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationY, false) : RoundToTileSizeMultiple(SplitLocationY, true);
	
	FColor NewRoomColor = 	FColor::MakeRandomColor();
	//
	/*for(int i = 0; i < (RoomSizeX / TileSize); ++i)
	{
		FVector CurrentTilePos = FVector(RoomOrigin.X + (i * TileSize), SplitLocationY, RoomOrigin.Z);
		if(AActor* CurrentTile = SpawnTile(CurrentTilePos))
		{
			if(UTileComponent* CurrenTileComponent = CurrentTile->FindComponentByClass<UTileComponent>())
			{
				CurrenTileComponent->SetTileTypeToWall();
			}
		}
	}*/
	//
	
	// Setup and spawn of Exit actor along split
	int RandLocationAlongSplit;
	if(RoomData.BSPSplitsRemaining == 1)
	{
		RandLocationAlongSplit = RoomData.SizeX * FMath::RandRange(0.225, 0.775);
	}
	else
	{
		RandLocationAlongSplit = RoomData.SizeX *
			(FMath::RandBool() ? FMath::RandRange(LowerMin, LowerMax) : FMath::RandRange(UpperMin, UpperMax));
	}
	
	int ExitSpawnPosX = RoomData.Origin.X + RoundToTileSizeMultiple(RandLocationAlongSplit, true);

	FVector ExitSpawnPos = FVector(ExitSpawnPosX, SplitLocationY, RoomData.Origin.Z);
	TObjectPtr<AExitGenerator> Exit = MapGenerator->GetWorld()->SpawnActor<AExitGenerator>(AExitGenerator::StaticClass(), ExitSpawnPos, FRotator(0, 0, 0));
	Exit->Init(TileSize);
	MapGenerator->AllRoomExits.Add(Exit);

	// Creating and initialising child rooms
	LeftChildRoom = NewObject<UMapRoom>();
	if(IsValid(LeftChildRoom))
	{
		FRoomData LeftRoomData(FVector(RoomData.Origin.X, RoomData.Origin.Y, RoomData.Origin.Z + RoomSpawnOffsetZ),
			RoomData.SizeX, CalculateLeftRoomSize(SplitLocationY, RoomData.Origin.Y), RoomData.BSPSplitsRemaining - 1,
			RoomData.MinPadding, RoomData.MaxPadding);
		LeftChildRoom->InitRoom(MapGenerator, this, LeftRoomData);
	}

	RightChildRoom = NewObject<UMapRoom>();
	if(IsValid(RightChildRoom))
	{
		int RightRoomOriginY = SplitLocationY + TileSize;
		FRoomData RightRoomData(FVector(RoomData.Origin.X, RightRoomOriginY, RoomData.Origin.Z + RoomSpawnOffsetZ),
			RoomData.SizeX, CalculateRightRoomSize(RightRoomOriginY, RoomBottom), RoomData.BSPSplitsRemaining - 1,
			RoomData.MinPadding, RoomData.MaxPadding);
		RightChildRoom->InitRoom(MapGenerator, this, RightRoomData);
	}
}

void UMapRoom::SplitVertically(const float SplitPercent)
{
	int RoomRight = RoomData.Origin.X + RoomData.SizeX;
	
	int SplitLocationX = RoomData.Origin.X + ((RoomRight - RoomData.Origin.X) * SplitPercent);
	SplitLocationX = (SplitPercent >= 0.5f) ? RoundToTileSizeMultiple(SplitLocationX, false) : RoundToTileSizeMultiple(SplitLocationX, true);
	
	FColor NewRoomColor = 	FColor::MakeRandomColor();
	//
	/*for(int i = 0; i < (RoomSizeY / TileSize); ++i)
	{
		FVector CurrentTilePos = FVector(SplitLocationX, RoomOrigin.Y + (i * TileSize), RoomOrigin.Z);

		if(AActor* CurrentTile = SpawnTile(CurrentTilePos))
		{
			if(UTileComponent* CurrenTileComponent = CurrentTile->FindComponentByClass<UTileComponent>())
			{
				CurrenTileComponent->SetTileTypeToWall();
			}
		}
	}*/
	//
	UE_LOG(LogTemp, Warning, TEXT(""));
	int RandLocationAlongSplit;
	if(RoomData.BSPSplitsRemaining == 1)
	{
		RandLocationAlongSplit = RoomData.SizeY * FMath::RandRange(0.225, 0.775);
	}
	else
	{
		RandLocationAlongSplit = RoomData.SizeY *
			(FMath::RandBool() ? FMath::RandRange(LowerMin, LowerMax) : FMath::RandRange(UpperMin, UpperMax));
	}
	
	int ExitSpawnPosY = RoomData.Origin.Y + RoundToTileSizeMultiple(RandLocationAlongSplit, true);
	
	FVector ExitSpawnPos = FVector(SplitLocationX, ExitSpawnPosY, RoomData.Origin.Z);
	TObjectPtr<AExitGenerator> Exit = MapGenerator->GetWorld()->SpawnActor<AExitGenerator>(AExitGenerator::StaticClass(), ExitSpawnPos, FRotator(0, 90, 0));
	Exit->Init(TileSize);
	MapGenerator->AllRoomExits.Add(Exit);

	// Creating and initialising child rooms
	LeftChildRoom = NewObject<UMapRoom>();
	if(IsValid(LeftChildRoom))
	{
		FRoomData LeftRoomData(FVector(RoomData.Origin.X, RoomData.Origin.Y, RoomData.Origin.Z + RoomSpawnOffsetZ),
		CalculateLeftRoomSize(SplitLocationX, RoomData.Origin.X), RoomData.SizeY, RoomData.BSPSplitsRemaining - 1,
		RoomData.MinPadding, RoomData.MaxPadding);
		
		LeftChildRoom->InitRoom(MapGenerator, this, LeftRoomData);
	}
	
	RightChildRoom = NewObject<UMapRoom>();
	if(IsValid(RightChildRoom))
	{
		int RightRoomOriginX = SplitLocationX + TileSize;
		FRoomData RightRoomData(FVector(RightRoomOriginX, RoomData.Origin.Y, RoomData.Origin.Z + RoomSpawnOffsetZ),
		CalculateRightRoomSize(RightRoomOriginX, RoomRight), RoomData.SizeY, RoomData.BSPSplitsRemaining - 1,
		RoomData.MinPadding, RoomData.MaxPadding);
		
		RightChildRoom->InitRoom(MapGenerator, this, RightRoomData);
	}
}

void UMapRoom::GenerateTiles()
{
	int LeftPadding = FMath::RandRange(RoomData.MinPadding, RoomData.MaxPadding) * TileSize;
	int TopPadding = FMath::RandRange(RoomData.MinPadding, RoomData.MaxPadding) * TileSize;
	int RightPadding = FMath::RandRange(RoomData.MinPadding, RoomData.MaxPadding) * TileSize;
	int BottomPadding = FMath::RandRange(RoomData.MinPadding, RoomData.MaxPadding) * TileSize;

	
	FVector RoomStartPos = FVector(RoomData.Origin.X + LeftPadding, RoomData.Origin.Y + TopPadding, RoomData.Origin.Z);
	FVector RoomEndPos = FVector((RoomStartPos.X + RoomData.SizeX) - RightPadding, (RoomStartPos.Y + RoomData.SizeY) - BottomPadding, RoomStartPos.Z);
	
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
	int TileMapArrayIndex = UTileMapFunctionLibrary::CalculateIndexFromTilePos(TileSpawnPos, MapGenerator->MapOrigin, MapGenerator->MapSizeX, TileSize);
	TObjectPtr<AActor> CurrentTile;
	if(TileMapArrayIndex >= MapGenerator->MapTiles.Num())
	{
		DrawDebugLine(MapGenerator->GetOwner()->GetWorld(), TileSpawnPos, FVector(TileSpawnPos.X, TileSpawnPos.Y, TileSpawnPos.Z + 1000.f), FColor::Red, false, 6000, 0, 10.f);
		UE_LOG(LogTemp, Display, TEXT("Out of bounds!"));
	}
	else
	{
		// Spawns new tile actor if it does not exist inside the "MapTiles" array
		if(!IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
		{
			if(IsValid(MapGenerator->GetWorld()))
			{
				FActorSpawnParameters SpawnParameters;
				SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				CurrentTile = MapGenerator->GetWorld()->SpawnActor<AActor>(MapGenerator->TileClass, TileSpawnPos, FRotator::ZeroRotator, SpawnParameters);

				if (IsValid(CurrentTile))
				{
					MapGenerator->MapTiles[TileMapArrayIndex] = CurrentTile;
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("CurrentTile Invalid...%f, %f, %f"), TileSpawnPos.X, TileSpawnPos.Y, TileSpawnPos.Z);
				}
			}
		}
			
		// Initialises the current tile component
		if(IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
		{
			CurrentTile = MapGenerator->MapTiles[TileMapArrayIndex];
			CurrentTile->SetActorLocation(TileSpawnPos);

			if(CurrentTile->Implements<UIsTile>())
			{
				if(UTileComponent* CurrentTileComponent = IIsTile::Execute_GetTileComponent(CurrentTile))
				{
					CurrentTileComponent->InitTile(this, TileSize, RoomIndexX, RoomIndexY);
				}
				else
				{
					UE_LOG(LogTemp, Display, TEXT("No tile component..."));
				}
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
