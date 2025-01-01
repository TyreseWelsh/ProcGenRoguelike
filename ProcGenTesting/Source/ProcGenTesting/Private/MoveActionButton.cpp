// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveActionButton.h"

#include "PlayerActionMove.h"
#include "StrategyPlayerController.h"
#include "TileComponent.h"
#include "Kismet/GameplayStatics.h"

void UMoveActionButton::Init(AActor* NewOwner)
{
	Owner = NewOwner;
}

void UMoveActionButton::OnButtonPress()
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

void UMoveActionButton::EnableButton()
{
	bEnabled = true;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString(TEXT("Move button enabled...")));
}

void UMoveActionButton::DisableButton()
{
	bEnabled = false;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("Move button disabled...")));
}
