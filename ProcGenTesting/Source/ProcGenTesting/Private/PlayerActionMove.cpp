// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionMove.h"

#include "HasPathfinding.h"
#include "PathfindingComponent.h"
#include "TileComponent.h"
#include "StrategyPlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "IsTile.h"
#include "TileMapFunctionLibrary.h"

void UPlayerActionMove::Init(AActor* NewUnit)
{	
	SetMovingUnit(NewUnit);
	StartPlanningMovement();
}

void UPlayerActionMove::OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile)
{
	if(bPlanningMove && bCanHover)
	{
		if(IsValid(UnitStartingTile)
			&& IsValid(UnitPathfindingComponent)
			&& CurrentHoveredTile != NewHoveredTile)
		{
			UnitPathfindingComponent->AttemptPathfinding(UnitStartingTile, NewHoveredTile);
			UnitPathfindingComponent->HighlightTiles(UnitPathfindingComponent->FindValidTiles(), FLinearColor::Green);
		}
	}
}

void UPlayerActionMove::OnLeftClick(UTileComponent* SelectedTile)
{
	if(bPlanningMove)
	{
		StartMove(SelectedTile);
	}
}

void UPlayerActionMove::OnRightClick()
{
	if(bPlanningMove)
	{
		CancelMove();
	}
}

void UPlayerActionMove::SetMovingUnit(AActor* NewUnit)
{
	MovingUnit = NewUnit;

	SetPathfindingComponent();
    SetStartingTile();
}

void UPlayerActionMove::SetPathfindingComponent()
{
	if(IsValid(MovingUnit))
	{
		UnitPathfindingComponent = IHasPathfinding::Execute_GetPathfindingComponent(MovingUnit);
	}
}

void UPlayerActionMove::SetStartingTile()
{
	if(IsValid(MovingUnit))
	{
		UnitStartingTile = IIsTile::Execute_GetTileComponent(UTileMapFunctionLibrary::GetBelowTile(MovingUnit));
	}
}

// Called when move action clicked
void UPlayerActionMove::StartPlanningMovement()
{
	if(IsValid(MovingUnit) &&
	IsValid(UnitPathfindingComponent) &&
	IsValid(UnitStartingTile))
	{
		UnitPathfindingComponent->HighlightTilesInRange(UnitStartingTile, UnitPathfindingComponent->GetMoveDistance(), FLinearColor::Blue);
	}
}

// Called when a valid tile to move to is selected
void UPlayerActionMove::StartMove(UTileComponent* SelectedTile)
{
	bPlanningMove = false;
	if(IsValid(UnitPathfindingComponent))
	{
		UnitPathfindingComponent->UnHighlightTiles(UnitPathfindingComponent->GetTilesInRange(), FLinearColor::Blue);
		
		// Call unit PathfindingComponent Move function
		UnitPathfindingComponent->GetMovementEndDelegate()->BindUObject(this, &UPlayerActionMove::EndAction);
		UnitPathfindingComponent->Move();
	}
}

void UPlayerActionMove::CancelMove()
{
}

void UPlayerActionMove::EndAction()
{
	//
	bPlanningMove = true;
	if(ActionEndDelegate.IsBound())
	{
		ActionEndDelegate.Broadcast();
	}
	ActionEndDelegate.RemoveAll(this);
}

void UPlayerActionMove::FindMovementTiles()
{
	/*if(IsValid(UnitStartingTile) &&
		IsValid(StrategyController) &&
		IsValid(UnitPathfindingComponent))
	{
		UnitPathfindingComponent->AttemptPathfinding(UnitStartingTile, StrategyController->GetCurrentHoveredTileComponent());
		UnitPathfindingComponent->HighlightTiles(UnitPathfindingComponent->FindValidTiles(), FLinearColor::Green);
	}*/
}
