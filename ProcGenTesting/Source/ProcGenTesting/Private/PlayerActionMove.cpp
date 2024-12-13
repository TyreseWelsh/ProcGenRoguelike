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
	//PlanMoveTimerDelegate.BindUFunction(this, "FindMovementTiles");

	
	SetMovingUnit(NewUnit);
	StartMove();
}

void UPlayerActionMove::OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile)
{
	if(bPlanningMove)
	{
		if(IsValid(UnitStartingTile) &&
			IsValid(UnitPathfindingComponent))
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
		EndMove(SelectedTile);
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

void UPlayerActionMove::StartMove()
{
	if(IsValid(MovingUnit) &&
	IsValid(UnitPathfindingComponent) &&
	IsValid(UnitStartingTile))
	{
		UnitPathfindingComponent->HighlightTilesInRange(UnitStartingTile, UnitPathfindingComponent->GetMoveDistance(), FLinearColor::Blue);
		//MovingUnit->GetWorld()->GetTimerManager().SetTimer(PlanMoveTimerHandle, PlanMoveTimerDelegate, 0.05f, true);
	}
}

void UPlayerActionMove::EndMove(UTileComponent* SelectedTile)
{
	bPlanningMove = false;
	UnitPathfindingComponent->UnHighlightTiles(UnitPathfindingComponent->GetTilesInRange(), FLinearColor::Blue);
	
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString(TEXT("Unit Moving...")));
	// Call unit PathfindingComponent Move function
}

void UPlayerActionMove::CancelMove()
{
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
