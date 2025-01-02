// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TWUserWidget.h"
#include "UnitInfoBar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInfoBarHoveredSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInfoBarUnHoveredSignature);

/**
 * 
 */
UCLASS()
class TYRESESUI_API UUnitInfoBar : public UTWUserWidget
{
	GENERATED_BODY()

protected:
	// Will be called when button is hovered to broadcast that UI has been hovered
	UFUNCTION(BlueprintCallable)
	void BroadcastOnHovered();
	UFUNCTION(BlueprintCallable)
	void BroadcastOnUnHovered();
	
public:
	void Init(AActor* NewOwner);
	void MoveAction();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwner() { return Owner; }

	// Delegate to bind to Controller function (to remove current hovered tile)
	FOnInfoBarHoveredSignature* GetOnHoveredDelegate() { return &OnHoveredDelegate; }
	FOnInfoBarUnHoveredSignature* GetOnUnHoveredDelegate() { return &OnUnHoveredDelegate; }
	
private:
	UPROPERTY()
	TObjectPtr<AActor> Owner;

	FOnInfoBarHoveredSignature OnHoveredDelegate;
	FOnInfoBarUnHoveredSignature OnUnHoveredDelegate;
};
