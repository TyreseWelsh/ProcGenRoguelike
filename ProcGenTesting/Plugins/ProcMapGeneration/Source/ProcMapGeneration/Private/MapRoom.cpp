// Fill out your copyright notice in the Description page of Project Settings.


#include "MapRoom.h"

#include "MapGeneratorComponent.h"
#include "Engine/World.h"
#include "TileComponent.h"
#include "IsTile.h"
#include "ExitGenerator.h"
#include "TileMapFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "BattleManager.h"

void AMapRoom::Init(FRoomData* NewRoomData, UMapGeneratorComponent* MapGeneratorComponent)
{
	RoomData = NewRoomData;
	MapGenerator = MapGeneratorComponent;
	
	// From here we can setup room type and generate relevant tiles and objects
	GenerateTiles();
}

void AMapRoom::GenerateTiles()
{
	int LeftPadding = FMath::RandRange(RoomData->MinPadding, RoomData->MaxPadding) * TileSize;
	int TopPadding = FMath::RandRange(RoomData->MinPadding, RoomData->MaxPadding) * TileSize;
	int RightPadding = FMath::RandRange(RoomData->MinPadding, RoomData->MaxPadding) * TileSize;
	int BottomPadding = FMath::RandRange(RoomData->MinPadding, RoomData->MaxPadding) * TileSize;

	
	FVector RoomStartPos = FVector(RoomData->Origin.X + LeftPadding, RoomData->Origin.Y + TopPadding, RoomData->Origin.Z);
	FVector RoomEndPos = FVector((RoomStartPos.X + RoomData->SizeX) - RightPadding, (RoomStartPos.Y + RoomData->SizeY) - BottomPadding, RoomStartPos.Z);
	
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
}

AActor* AMapRoom::SpawnTile(FVector TileSpawnPos, int RoomIndexX, int RoomIndexY)
{
	if(IsValid(MapGenerator))
	{
		int TileMapArrayIndex = UTileMapFunctionLibrary::CalculateIndexFromTilePos(TileSpawnPos, MapGenerator->MapOrigin, MapGenerator->MapSizeX, TileSize);
		TObjectPtr<AActor> CurrentTile;
		if(TileMapArrayIndex >= MapGenerator->MapTiles.Num())
		{
			DrawDebugLine(GetWorld(), TileSpawnPos, FVector(TileSpawnPos.X, TileSpawnPos.Y, TileSpawnPos.Z + 1000.f), FColor::Red, false, 6000, 0, 10.f);
			UE_LOG(LogTemp, Display, TEXT("Out of bounds!"));
		}
		else
		{
			// Spawns new tile actor if it does not exist inside the "MapTiles" array
			if(!IsValid(MapGenerator->MapTiles[TileMapArrayIndex]))
			{
				if(IsValid(GetWorld()))
				{
					FActorSpawnParameters SpawnParameters;
					SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					CurrentTile = GetWorld()->SpawnActor<AActor>(TileClass, TileSpawnPos, FRotator::ZeroRotator, SpawnParameters);

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

	UE_LOG(LogTemp, Display, TEXT("AMapRoom - MapGenerator Null..."));
	return nullptr;
}

void AMapRoom::Enter()
{
}

void AMapRoom::Activate()
{
	RoomBattleManager = GetWorld()->SpawnActor<ABattleManager>(BattleManagerClass, GetActorLocation(), FRotator::ZeroRotator);
	if(RoomBattleManager)
	{
		RoomBattleManager->Init(RoomObjects);
		RoomBattleManager->Activate();
	}
}
