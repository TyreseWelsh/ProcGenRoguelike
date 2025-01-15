// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomContentsManager.generated.h"

class AMapRoom;
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

	AMapRoom* FindSpawnRoom(TArray<AMapRoom*> MapRooms);
	bool SpawnUnit(AMapRoom* SpawnRoom, int SpawnAttempts);

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PlayerCharacterClass;

	TArray<ATBActor*> RoomObjects;
};
