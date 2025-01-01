// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionOpen.h"

#include "TileComponent.h"

void UPlayerActionOpen::Init(AActor* NewUnit)
{
	Super::Init(NewUnit);
}

void UPlayerActionOpen::End()
{
}
// ERROR: I changed the assigning of CurrentHoveredTile from controller tick to here which caused problems
// Apparently changing the value of a pointer in this function, does not change the value of the pointer in its owning class (StrategyPlayerController)
void UPlayerActionOpen::OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile)
{
	Super::OnHover(CurrentHoveredTile, NewHoveredTile);

	if(CurrentHoveredTile != NewHoveredTile)
	{
		// Unhovering previous hovered tile first
		if(CurrentHoveredTile != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString::Printf(TEXT("UNHOVER")));

			CurrentHoveredTile->TileUnHover();
		}
		
		NewHoveredTile->TileHover();
	}
}

void UPlayerActionOpen::OnLeftClick(UTileComponent* SelectedTile)
{
	Super::OnLeftClick(SelectedTile);

	SelectedTile->TileLeftClick();
}

void UPlayerActionOpen::OnRightClick()
{
	Super::OnRightClick();
}
