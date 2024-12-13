// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomContentsManager.generated.h"

class UMapRoom;

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

	void FindSpawnRoom(TArray<UMapRoom*> MapRooms);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PlayerCharacterClass;
};
