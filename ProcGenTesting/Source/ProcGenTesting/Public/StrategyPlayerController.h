// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StrategyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UTileComponent;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API AStrategyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	
	
private:
	// Input
	void PlayerLeftClick();
	void PlayerRightClick();
	
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftClickAction;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightClickAction;


	//
	APawn* PlayerCam;

	UPROPERTY()
	UTileComponent* CurrentHoveredTileComponent;
	UPROPERTY()
	UTileComponent* CurrentSelectedTileComponent;
};
