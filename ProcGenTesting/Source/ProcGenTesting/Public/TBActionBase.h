// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBActionBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionEndSignature);

class AActor;
class UTileComponent;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API UTBActionBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* NewUnit);
	UFUNCTION()
	virtual void End();
	virtual void OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile);
	virtual void OnLeftClick(UTileComponent* SelectedTile);
	virtual void OnRightClick();

	virtual void DisableHover(UTileComponent* CurrentHoveredTile);
	virtual void EnableHover();

	FOnActionEndSignature* GetActionEndDelegate() { return &ActionEndDelegate; }
	
protected:
	bool bCanHover = true;
	
	FOnActionEndSignature ActionEndDelegate;
};
