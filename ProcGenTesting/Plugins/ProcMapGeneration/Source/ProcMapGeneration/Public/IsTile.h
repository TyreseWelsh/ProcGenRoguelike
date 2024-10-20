// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsTile.generated.h"

class UTileComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsTile : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROCMAPGENERATION_API IIsTile
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UTileComponent* GetTileComponent();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddTileColour(FLinearColor NewColour);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SubtractTileColour(FLinearColor NewColour);
};
