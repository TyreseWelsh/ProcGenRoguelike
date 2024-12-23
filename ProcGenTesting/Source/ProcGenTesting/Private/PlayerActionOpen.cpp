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

	if(NewHoveredTile != CurrentHoveredTile)
	{
		// Unhovering previous hovered tile first
		if(IsValid(CurrentHoveredTile))
		{
			CurrentHoveredTile->TileUnHover();
		}

		NewHoveredTile->TileHover();
	}
}

void UPlayerActionOpen::OnLeftClick(UTileComponent* SelectedTile)
{
	Super::OnLeftClick(SelectedTile);

	if(IsValid(SelectedTile))
	{
		SelectedTile->TileUnSelect();
	}

	SelectedTile->TileLeftClick();
}

void UPlayerActionOpen::OnRightClick()
{
	Super::OnRightClick();
}
