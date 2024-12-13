// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveActionButton.h"

#include "PlayerActionMove.h"
#include "StrategyPlayerController.h"
#include "TileComponent.h"
#include "Kismet/GameplayStatics.h"

void UMoveActionButton::OnButtonPress()
{
	if(AStrategyPlayerController* StrategyController = Cast<AStrategyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		TObjectPtr<UPlayerActionMove> NewMoveAction = NewObject<UPlayerActionMove>(); 
		StrategyController->SetCurrentAction(NewMoveAction);
		if(StrategyController->GetCurrentAction())
		{
			StrategyController->GetCurrentAction()->Init(StrategyController->GetCurrentSelectedTileComponent()->GetOccupyingObject());
		}
	}
}
