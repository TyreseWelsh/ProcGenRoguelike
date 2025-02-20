// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionButton.h"
#include "MoveActionButton.generated.h"

class UTWButton;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API UMoveActionButton : public UActionButton
{
	GENERATED_BODY()



public:
	UFUNCTION(BlueprintCallable)
	void OnButtonPress();

	UFUNCTION()
	void EnableButton();
	UFUNCTION()
	void DisableButton();
	
private:
	bool bEnabled = true;
};
