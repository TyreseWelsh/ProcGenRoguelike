// Fill out your copyright notice in the Description page of Project Settings.


#include "TBActionBase.h"

#include "TileComponent.h"

void UTBActionBase::Init(AActor* NewUnit)
{
}

void UTBActionBase::End()
{
}

void UTBActionBase::OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile)
{
}

void UTBActionBase::OnLeftClick(UTileComponent* SelectedTile)
{
}

void UTBActionBase::OnRightClick()
{
}

void UTBActionBase::DisableHover(UTileComponent* CurrentHoveredTile)
{
	bCanHover = false;
	if(IsValid(CurrentHoveredTile))
	{
		CurrentHoveredTile->TileUnHover();
	}
}

void UTBActionBase::EnableHover()
{
	bCanHover = true;
}
