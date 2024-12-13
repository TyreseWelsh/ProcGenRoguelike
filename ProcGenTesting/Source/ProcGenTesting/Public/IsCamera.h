// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IsCamera.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIsCamera : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROCGENTESTING_API IIsCamera
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Pan();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Rotate();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Zoom();
};
