// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TWButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonHoverSignature);

/**
 * 
 */
UCLASS()
class TYRESESUI_API UTWButton : public UButton
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void BroadcastButtonHovered() const;
	
	FOnButtonHoverSignature* GetOnButtonHoverDelegate() { return &OnButtonHoverDelegate; }
	UPROPERTY(BlueprintAssignable)
	FOnButtonHoverSignature OnButtonHoverDelegate;

private:
};
