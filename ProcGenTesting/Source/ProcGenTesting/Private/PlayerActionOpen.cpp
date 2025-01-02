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
