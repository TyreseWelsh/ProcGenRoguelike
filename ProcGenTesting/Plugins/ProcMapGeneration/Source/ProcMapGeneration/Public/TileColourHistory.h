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
	bool Add(UTileColour* NewColour, FLinearColor& OutColour);
	bool Remove(UTileColour* NewColour, FLinearColor& OutColour);
	
private:
	bool EvaluateColour(FLinearColor& OutColour);

	//UPROPERTY()
	TArray<UTileColour*> ColourHistory;
};
