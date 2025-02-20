// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveActionButton.h"

#include "PlayerActionMove.h"
#include "TWButton.h"
#include "StrategyPlayerController.h"
#include "TileComponent.h"
#include "Kismet/GameplayStatics.h"


void UMoveActionButton::OnButtonPressed()
{
	if(bEnabled && IsValid(Owner))
	{
		if(AStrategyPlayerController* StrategyController = Cast<AStrategyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			TObjectPtr<UPlayerActionMove> NewMoveAction = NewObject<UPlayerActionMove>(); 
			StrategyController->SetCurrentAction(NewMoveAction);
			if(StrategyController->GetCurrentAction())
			{
				StrategyController->GetCurrentAction()->GetActionEndDelegate()->AddUObject(this, &UMoveActionButton::EnableButton);
				DisableButton();
				
				StrategyController->GetCurrentAction()->Init(Owner);
			}
		}
	}
}