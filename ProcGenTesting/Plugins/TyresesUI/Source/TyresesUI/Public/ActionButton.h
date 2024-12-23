// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TWUserWidget.h"
#include "ActionButton.generated.h"

/**
 * 
 */
UCLASS()
class TYRESESUI_API UActionButton : public UTWUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OnButtonClicked(FString ClickedActionName);
};
