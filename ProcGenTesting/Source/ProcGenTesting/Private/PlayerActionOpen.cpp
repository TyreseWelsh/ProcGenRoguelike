// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionOpen.h"

#include "TileComponent.h"

void UPlayerActionOpen::Init(AActor* NewUnit)
{
	Super::Init(NewUnit);
}

void UPlayerActionOpen::EndAction()
{
}

void UPlayerActionOpen::OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile)
{
	Super::OnHover(CurrentHoveredTile, NewHoveredTile);

	if(CurrentHoveredTile != NewHoveredTile)
	{
		// Unhovering previous hovered tile first
		if(CurrentHoveredTile != nullptr)
		{
			CurrentHoveredTile->TileUnHover();
		}

		if(bCanHover)
		{
			NewHoveredTile->TileHover();
			GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, "LAG!");

		}
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
