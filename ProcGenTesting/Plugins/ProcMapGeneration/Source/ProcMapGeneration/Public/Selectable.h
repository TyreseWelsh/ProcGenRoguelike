// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Selectable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USelectable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROCMAPGENERATION_API ISelectable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMouseHover();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMouseUnHover();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMouseLeft();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMouseRight();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMouseUnSelect();

	bool bCanBeSelected;
};
