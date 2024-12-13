// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HasActionStates.h"
#include "GameFramework/PlayerController.h"
#include "StrategyPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UTileComponent;
class UTBActionBase;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API AStrategyPlayerController : public APlayerController, public IHasActionStates
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION()
	void SetCurrentAction(UTBActionBase* NewAction) { CurrentAction = NewAction; }
	UFUNCTION()
	UTBActionBase* GetCurrentAction() const { return CurrentAction; }
	UFUNCTION()
	UTileComponent* GetCurrentHoveredTileComponent() const { return CurrentHoveredTileComponent; }
	UFUNCTION()
	UTileComponent* GetCurrentSelectedTileComponent() const { return CurrentSelectedTileComponent; }
	
private:
	// Input
	void PlayerLeftClick();
	void PlayerRightClick();

	// Saves the current mouse screen position
	void SaveInitialMousePosition();

	void MoveMouse(const FInputActionValue& Value);

	void EnableCameraRotation();
	void DisabledCameraRotation();

	void EnableCameraPan();
	void DisableCameraPan();
	
	void CameraZoom(const FInputActionValue& Value);

	void FindPathFromSelected();
	
	UPROPERTY(EditDefaultsOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* DefaultLeftClickAction;

	UPROPERTY(EditDefaultsOnly, Category = DefaultInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* DefaultRightClickAction;

	UPROPERTY(EditDefaultsOnly, Category = CameraInput, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* CameraMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = CameraInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraMouseMoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = CameraInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraRightClickAction;

	UPROPERTY(EditDefaultsOnly, Category = CameraInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraMiddleClickAction;

	UPROPERTY(EditDefaultsOnly, Category = CameraInput, meta = (AllowPrivateAccess = "true"))
	UInputAction* CameraMiddleScrollAction;

	//EPlayerState CurrentPlayerState = EPlayerState::Open;
	UPROPERTY()
	TObjectPtr<UTBActionBase> CurrentAction;
	//
	FVector ScreenMousePosition;
	FVector InitialScreenMousePosition;

	FVector WorldMousePosition;
	FVector WorldMouseDirection;
	UPROPERTY()
	APawn* PlayerCam;
	bool bCanRotateCamera = false;
	bool bCanPanCamera = false;

	UPROPERTY()
	UTileComponent* CurrentHoveredTileComponent;
	UPROPERTY()
	UTileComponent* CurrentSelectedTileComponent;
};
