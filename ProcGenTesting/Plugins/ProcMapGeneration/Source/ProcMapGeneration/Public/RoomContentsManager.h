// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomContentsManager.generated.h"

class UMapRoom;
class ATBActor;

/**
 * 
 */
UCLASS(Blueprintable)
class PROCMAPGENERATION_API URoomContentsManager : public UObject
{
	GENERATED_BODY()
	
public:
	URoomContentsManager();
	//~RoomContentsManager();

	UMapRoom* FindSpawnRoom(TArray<UMapRoom*> MapRooms);
	bool SpawnUnit(UMapRoom* SpawnRoom, int SpawnAttempts);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PlayerCharacterClass;

	TArray<ATBActor*> RoomObjects;
};
