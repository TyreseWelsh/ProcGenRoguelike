// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomContentsManager.h"

#include "IsTile.h"
#include "MapRoom.h"
#include "TileComponent.h"
#include "TileMapFunctionLibrary.h"
#include "TBActor.h"

URoomContentsManager::URoomContentsManager()
{
}

AMapRoom* URoomContentsManager::FindSpawnRoom(TArray<AMapRoom*> MapRooms)
{
	// Loop through rooms, choosing one as the start room to spawn the player, and start from there
	if(!MapRooms.IsEmpty())
	{
		int RoomNum = FMath::RandRange(0, MapRooms.Num() - 1);
		AMapRoom* SpawnRoom = MapRooms[0];	// MapRooms[RoomNum]

		// NOTE: TEMPORARY unit spawn
		int UnitsToSpawn = 5;
		for (int i = 0; i < UnitsToSpawn; ++i)
		{
			SpawnUnit(SpawnRoom, 10);
		}

		return SpawnRoom;
	}

	return nullptr;
}

bool URoomContentsManager::SpawnUnit(AMapRoom* SpawnRoom, int SpawnAttempts)
{
	if(SpawnAttempts <= 0)
	{
		return false;
	}
	
	int TileNum = FMath::RandRange(0, SpawnRoom->GetRoomTiles().Num() - 1);
	AActor* SpawnTile = SpawnRoom->GetRoomTiles()[TileNum];

	if(UTileComponent* SpawnTileComponent = Cast<UTileComponent>(IIsTile::Execute_GetTileComponent(SpawnTile)))
	{
		// If spawn tile is not occupied, spawn unit
		if(!SpawnTileComponent->GetOccupyingObject())
		{
			FActorSpawnParameters SpawnParameters;
			SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
			if(IsValid(PlayerCharacterClass))
			{
				ATBActor* SpawnedActor = SpawnTile->GetWorld()->SpawnActor<ATBActor>(PlayerCharacterClass, SpawnTile->GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
				UTileMapFunctionLibrary::OccupyTile(SpawnedActor);
				SpawnRoom->GetRoomObjects().Add(SpawnedActor);
				return true;
			}

			return false;
		}
		
		return SpawnUnit(SpawnRoom, SpawnAttempts - 1);
	}

	// "SpawnTile" does not have a UTileComponent
	return false;
}
