// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBGameMode.generated.h"

class AMapRom;

/**
 * 
 */
UCLASS()
class PROCMAPGENERATION_API ATBGameMode : public AGameModeBase
{
	GENERATED_BODY()

	TObjectPtr<AMapRom> CurrentRoom;
};
