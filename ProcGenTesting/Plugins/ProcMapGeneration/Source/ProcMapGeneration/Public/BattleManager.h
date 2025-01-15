// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BattleManager.generated.h"

class UUserWidget;
enum class EBattlePriority : uint8;
class ATBActor;
class UBattleTimeline;

/**
 * 
 */
UCLASS()
class PROCMAPGENERATION_API UBattleManager : public UObject
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> BattleTimelineClass;
	TObjectPtr<UBattleTimeline> BattleTimeline;
	
	TArray<TArray<ATBActor*>> CurrentBattleObjects;
	int PriorityIndex = 0;
	int ObjectIndex = 0;

	bool InitTurnObject(ATBActor* TurnActor);

	
public:
	/*UBattleManager() {}
	UBattleManager(TArray<ATBActor*> TurnActors);*/
	
	void Init(TArray<ATBActor*> TurnActors);
	bool AddBattleObject(EBattlePriority TurnPriority, ATBActor* NewTurnObject);

	void Activate();
	void Deactivate();
	
	void InitNextTurn();
	void StartTurn();
	void EndTurn();
};
