// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoBar.h"

//#include "../../../../ProcMapGeneration/Source/ProcMapGeneration/Public/TBActor.h"
#include "../../ProcMapGeneration/Source/ProcMapGeneration/Public/TBActor.h"

void UUnitInfoBar::Init(AActor* NewOwner)
{
	Owner = Cast<ATBActor>(NewOwner);
}

void UUnitInfoBar::BroadcastOnHovered()
{
	if(OnHoveredDelegate.IsBound())
	{
		OnHoveredDelegate.Broadcast();
	}
}

void UUnitInfoBar::BroadcastOnUnHovered()
{
	if(OnUnHoveredDelegate.IsBound())
	{
		OnUnHoveredDelegate.Broadcast();
	}
}

void UUnitInfoBar::EndTurn()
{
	if(Owner)
	{
		Owner->EndTurn();
	}
}
