// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBActionBase.h"
#include "PlayerActionMove.generated.h"

class AActor;
class UTileComponent;
class UPathfindingComponent;
class UTileComponent;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API UPlayerActionMove : public UTBActionBase
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* NewUnit) override;
	virtual void OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile) override;
	virtual void OnLeftClick(UTileComponent* SelectedTile) override;
	virtual void OnRightClick() override;
	
	void SetMovingUnit(AActor* NewUnit);
	void SetPathfindingComponent();
	void SetStartingTile();

	void StartPlanningMovement();
	void StartMove(UTileComponent* SelectedTile);
	void CancelMove();
	virtual void End() override;
	
private:
	UFUNCTION()
	void FindMovementTiles();
	
	UPROPERTY()
	AActor* MovingUnit;
	UPROPERTY()
	UPathfindingComponent* UnitPathfindingComponent;
	UPROPERTY()
	UTileComponent* UnitStartingTile;

	bool bPlanningMove = true;
};
