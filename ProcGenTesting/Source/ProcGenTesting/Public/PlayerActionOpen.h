// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBActionBase.h"
#include "PlayerActionOpen.generated.h"

class AActor;
class UTileComponent;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API UPlayerActionOpen : public UTBActionBase
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* NewUnit) override;
	virtual void EndAction();
	virtual void OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile) override;
	virtual void OnLeftClick(UTileComponent* SelectedTile) override;
	virtual void OnRightClick() override;
};
