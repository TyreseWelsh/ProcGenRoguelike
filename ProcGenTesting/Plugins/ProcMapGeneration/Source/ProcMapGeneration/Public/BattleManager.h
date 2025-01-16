// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

class UTWUserWidget;
enum class EBattlePriority : uint8;
class ATBActor;
class UBattleTimeline;

/**
 * 
 */
UCLASS(Blueprintable)
class PROCMAPGENERATION_API ABattleManager : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UTWUserWidget> BattleTimelineClass;
	UBattleTimeline* BattleTimeline;
	
	TArray<TArray<ATBActor*>> CurrentBattleObjects;
	int PriorityIndex = 0;
	int ObjectIndex = 0;

	bool InitTurnObject(ATBActor* TurnActor);

	
public:
	void Init(TArray<ATBActor*> TurnActors);
	bool AddBattleObject(EBattlePriority TurnPriority, ATBActor* NewTurnObject);

	void Activate();
	void Deactivate();
	
	void InitNextTurn();
	void StartTurn();
	void EndTurn();
};
