// Fill out your copyright notice in the Description page of Project Settings.


#include "TWButton.h"

void UTWButton::BroadcastButtonHovered() const
{
	if(OnButtonHoverDelegate.IsBound())
	{
		OnButtonHoverDelegate.Broadcast();
	}
}
