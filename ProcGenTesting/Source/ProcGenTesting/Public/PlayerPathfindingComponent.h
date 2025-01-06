// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PathfindingComponent.h"
#include "PlayerPathfindingComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API UPlayerPathfindingComponent : public UPathfindingComponent
{
	GENERATED_BODY()

public:
	//
	UFUNCTION()
	void FindMovementTiles();
	UFUNCTION()
	void StartMove();
	
	virtual void Move() override;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void MoveToNextTile();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pathfinding", meta = (AllowPrivateAccess = "true"))
	FLinearColor MoveableColour;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Pathfinding", meta = (AllowPrivateAccess = "true"))
	FLinearColor UnMoveableColour;



	int TargetIndex = 0;
};
