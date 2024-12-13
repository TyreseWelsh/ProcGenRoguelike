// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileColourHistory.generated.h"

struct FLinearColor;
class UTileColour;

/**
 * 
 */
UCLASS()
class PROCMAPGENERATION_API UTileColourHistory : public UObject
{
	GENERATED_BODY()

public:
	FLinearColor Add(UTileColour* NewColour);
	FLinearColor Remove(UTileColour* NewColour);
	
private:
	FLinearColor EvaluateColour();

	//UPROPERTY()
	TArray<UTileColour*> ColourHistory;
};
