// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleManager.h"
#include "TBActor.h"
#include "BattleTimeline.h"
#include "Blueprint/UserWidget.h"

bool ABattleManager::InitBattleObject(ATBActor* BattleObject)
{
	if(BattleObject)
	{
		// Bind TurnActor delegates
		BattleObject->GetOnDeathDelegate()->AddUObject(this, &ABattleManager::RemoveBattleObject);
		return true;
	}

	return false;
}

void ABattleManager::RemoveBattleObject(ATBActor* ActorToRemove)
{
	//CurrentBattleObjects[(uint8)ActorToRemove->GetBattlePriority()].Remove(ActorToRemove);
	BattleObjects.Remove(ActorToRemove);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("BattleManager: Removed %s with BP: %i"), *ActorToRemove->GetName(), (uint8)ActorToRemove->GetBattlePriority()));
}

void ABattleManager::OrderBattleObject(EBattlePriority BattlePriority)
{
	//std::sort(CurrentBattleObjects.begin(), CurrentBattleObjects.end(), SortByDexterity);
	//CurrentBattleObjects.Sort([](const ATBActor& LHS, const ATBActor& RHS) { return LHS.GetDexterity() > RHS.GetDexterity(); });
	/*int Priority = (uint8)BattlePriority;
	bool bOrdered = false;
	if(!CurrentBattleObjects[Priority].IsEmpty())
	{
		while(!bOrdered)
		{
			
		}
	}
	CurrentBattleObjects[Priority].Sor*/
}

bool ABattleManager::SortByDexterity(const ATBActor& lhs, const ATBActor& rhs)
{
	return lhs.GetDexterity() > rhs.GetDexterity();
}

void ABattleManager::Init(TArray<ATBActor*> TurnActors)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow, FString::Printf(TEXT("Battle manager INIT")));

	for(ATBActor* TurnActor : TurnActors)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Adding turn object...")));
		AddBattleObject(TurnActor);
	}
}

// OLD
bool ABattleManager::AddBattleObject(EBattlePriority NewBattlePriority, ATBActor* NewBattleObject)
{
	switch(NewBattlePriority)
	{
		case(EBattlePriority::Player):
			CurrentBattleObjects[0].Add(NewBattleObject);
			InitBattleObject(NewBattleObject);
			break;
		case(EBattlePriority::Enemy):
			CurrentBattleObjects[1].Add(NewBattleObject);
			InitBattleObject(NewBattleObject);
			break;
		case(EBattlePriority::Neutral):
			CurrentBattleObjects[2].Add(NewBattleObject);
			InitBattleObject(NewBattleObject);
			break;
		case(EBattlePriority::Environment):
			CurrentBattleObjects[3].Add(NewBattleObject);
			InitBattleObject(NewBattleObject);
			break;
		default:
			// Dont add
		break;
	}
	
	return false;
}

void ABattleManager::AddBattleObject(ATBActor* NewBattleObject)
{
	BattleObjects.AddUnique(NewBattleObject);
	InitBattleObject(NewBattleObject);
	Algo::SortBy(BattleObjects, &ATBActor::GetDexterity, TGreater<>());

	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("BattleManager: Added %s"), *NewBattleObject->GetName()));
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
			BeginNewRound();
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
	BattleTimeline->RemoveFromParent();
}

// OLD
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

void ABattleManager::BeginNextTurn()
{
	if(IsValid(CurrentTurnObject))
	{
		CurrentTurnObject->GetOnTurnEndDelegate()->RemoveAll(this);
	}
	
	ObjectIndex++;
	if(ObjectIndex >= BattleObjects.Num())
	{
		// Start new round, resetting object index, updating UI, and using InitNextTurn() again
		BeginNewRound();
		return;
	}

	CurrentTurnObject = BattleObjects[ObjectIndex];
	CurrentTurnObject->GetOnTurnEndDelegate()->AddUObject(this, &ABattleManager::BeginNextTurn);
	CurrentTurnObject->StartTurn();
}

void ABattleManager::BeginNewRound()
{
	GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Orange, FString::Printf(TEXT("Starting new round...")));

	ObjectIndex = -1;

	// Update relevant round start UI

	BeginNextTurn();
}
