// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitTileNode.h"

void AExitTileNode::BindLeftClick()
{
	Super::BindLeftClick();

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Calling ExitTile LeftClick override...")));
}
