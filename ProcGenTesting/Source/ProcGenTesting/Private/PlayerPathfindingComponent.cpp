// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPathfindingComponent.h"
#include "TileNode.h"
#include "TileComponent.h"
#include "StrategyPlayerController.h"
#include "TileMapFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

// Called when the game starts
void UPlayerPathfindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	HighlightPathTimerDelegate.BindUFunction(this, "FindMovementTiles");
}

void UPlayerPathfindingComponent::FindMovementTiles()
{
	if(UTileComponent* OccupyingTileComponent = IIsTile::Execute_GetTileComponent(UTileMapFunctionLibrary::GetBelowTile(GetOwner())))
	{
		if(AStrategyPlayerController* StrategyController = Cast<AStrategyPlayerController>(UGameplayStatics::GetPlayerController(GetOwner()->GetWorld(), 0)))
		{
			AttemptPathfinding(OccupyingTileComponent, StrategyController->GetCurrentHoveredTileComponent());
			HighlightTiles(FindValidTiles(), FLinearColor::Green);
		}
	}
}

void UPlayerPathfindingComponent::StartMove()
{
	if(UTileComponent* OccupyingTileComponent = IIsTile::Execute_GetTileComponent(UTileMapFunctionLibrary::GetBelowTile(GetOwner())))
	{		
		HighlightTilesInRange(OccupyingTileComponent, GetMoveDistance(), FLinearColor::Blue);
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(HighlightPathTimerHandle, HighlightPathTimerDelegate, 0.05f, true);
	}
}