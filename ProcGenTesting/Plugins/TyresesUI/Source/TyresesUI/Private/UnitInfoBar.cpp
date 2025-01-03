// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoBar.h"

void UUnitInfoBar::Init(AActor* NewOwner)
{
	Owner = NewOwner;
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
