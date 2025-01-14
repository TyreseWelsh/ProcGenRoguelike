// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TBActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnTurnEndSignature);

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UENUM(BlueprintType)
enum class EBattlePriority : uint8
{
	None			UMETA(DisplayName="NONE"),
	Player			UMETA(DisplayName="PLAYER"),
	Enemy			UMETA(DisplayName="ENEMY"),
	Neutral			UMETA(DisplayName="NEUTRAL"),
	Environment		UMETA(DisplayName="ENVIRONMENT")
};

// This class will be the base for all objects/units placed in the map
// Features: Turn priority (player, enemy, neutral, environment), delegates to broadcast for key events such as turn end, on death/destruction, etc
UCLASS()
class PROCMAPGENERATION_API ATBActor : public AActor
{
	GENERATED_BODY()

	EBattlePriority BattlePriority = EBattlePriority::None;
	int Fatigue = 0;

	FOnTurnEndSignature OnTurnEndDelegate;
	FOnDeathSignature OnDeathDelegate;
	
public:	
	// Sets default values for this actor's properties
	ATBActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartTurn();
	void EndTurn();
	
	// Getters
	EBattlePriority GetBattlePriority() const { return BattlePriority; }
	
	FOnTurnEndSignature* GetOnTurnEndDelegate() { return &OnTurnEndDelegate; }
	FOnDeathSignature* GetOnDeathDelegate() { return &OnDeathDelegate; }
};
