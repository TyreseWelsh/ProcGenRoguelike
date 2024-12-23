// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomContentsManager.h"

#include "MapRoom.h"

URoomContentsManager::URoomContentsManager()
{
}

void URoomContentsManager::FindSpawnRoom(TArray<UMapRoom*> MapRooms)
{
	GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Green, FString::Printf(TEXT("Finding spawn room")));

	// Loop through rooms, choosing one as the start room to spawn the player, and start from there
	int RandNum = FMath::RandRange(0, MapRooms.Num() - 1);
	UMapRoom* SpawnRoom = MapRooms[0];

	RandNum = FMath::RandRange(0, SpawnRoom->GetRoomTiles().Num() - 1);
	AActor* SpawnTile = SpawnRoom->GetRoomTiles()[RandNum];

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	if(IsValid(PlayerCharacterClass))
	{
		AActor* SpawnedActor = SpawnTile->GetWorld()->SpawnActor<AActor>(PlayerCharacterClass, SpawnTile->GetActorLocation(), FRotator::ZeroRotator, SpawnParameters);
		//TBPlayer* SpawnedPlayer = Cast<PlayerCharacterClass>(SpawnedActor);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString::Printf(TEXT("Character class invalid")));
	}

}

/*URoomContentsManager::~RoomContentsManager()
{
}*/
