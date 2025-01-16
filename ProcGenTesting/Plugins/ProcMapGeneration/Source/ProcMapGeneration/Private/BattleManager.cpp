// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "TBActor.h"
#include "BattleTimeline.h"
#include "Blueprint/UserWidget.h"

bool ABattleManager::InitTurnObject(ATBActor* TurnActor)
{
	if(TurnActor)
	{
		// Bind TurnActor delegates
		// CurrentRoundObjects[PriorityIndex][ObjectIndex]->StartTurn();
		return true;
	}

	return false;
}

void ABattleManager::Init(TArray<ATBActor*> TurnActors)
{
	for(ATBActor* TurnActor : TurnActors)
	{
		AddBattleObject(TurnActor->GetBattlePriority(), TurnActor);
	}
}

bool ABattleManager::AddBattleObject(EBattlePriority NewBattlePriority, ATBActor* NewBattleObject)
{
	switch(NewBattlePriority)
	{
		case(EBattlePriority::Player):
			CurrentBattleObjects[0].Add(NewBattleObject);
			break;
		case(EBattlePriority::Enemy):
			CurrentBattleObjects[1].Add(NewBattleObject);
			break;
		case(EBattlePriority::Neutral):
			CurrentBattleObjects[2].Add(NewBattleObject);
			break;
		case(EBattlePriority::Environment):
			CurrentBattleObjects[3].Add(NewBattleObject);
			break;
		default:
			// Dont add
		break;
	}
	
	return false;
}

// Is called once the player enters a room that has not been cleared yet, starts the turn based combat
// Adds turn based UI to viewport at the top of the screen
// Plays relevant UI sounds and visuals/animations
void ABattleManager::Activate()
{
	if(BattleTimelineClass)
	{
		BattleTimeline = CreateWidget<UBattleTimeline>(GetWorld(), BattleTimelineClass);
		if(BattleTimeline)
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Orange, FString::Printf(TEXT("Added battle timeline to viewport")));

			BattleTimeline->AddToViewport();

			// Play BattleTimeline opening animation
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Orange, FString::Printf(TEXT("Battle timeline null...")));
		}
	}
}

// Is called once the player clears a room as turn based combat has ended
// Removes turn based UI from viewport
// Plays relevant UI sounds and visuals/animations
void ABattleManager::Deactivate()
{
}

// NOTE: TurnManager will subscribe to the current actors relevant delegates declared in "TBActor.h" such as:
//		 OnDeath-To remove the actor from the TurnObjects array, OnTurnEnd-To signal to the TurnManager that we should move to the next turn, etc
void ABattleManager::InitNextTurn()
{
	ObjectIndex++;
	// TRUE if all objects of this priority have taken action if able
	if(ObjectIndex >= CurrentBattleObjects[PriorityIndex].Num())
	{
		ObjectIndex = 0;
		PriorityIndex++;

		// TRUE if all objects in the room have taken action if able
		if(PriorityIndex >= CurrentBattleObjects.Num())
		{
			// GO TO NEXT "ROUND"
			// StartNextRound();
			return;
		}
	}

	// Subscribe functions to relevant TBActor delegates
	CurrentBattleObjects[PriorityIndex][ObjectIndex]->StartTurn();
}
