// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeleportPoint.h"
#include "RoomExit.generated.h"

/**
 * 
 */
UCLASS()
class PROCMAPGENERATION_API ARoomExit : public ATeleportPoint
{
	GENERATED_BODY()

public:
	virtual void TeleportObject(AActor* Object) override;
};
