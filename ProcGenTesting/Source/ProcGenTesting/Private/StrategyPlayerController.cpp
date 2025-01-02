// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "HasPathfinding.h"
#include "PathfindingComponent.h"
#include "TBActionBase.h"
#include "PlayerActionOpen.h"
#include "TileMapFunctionLibrary.h"
#include "ProcMapGeneration/Public/TileComponent.h"
#include "IsTile.h"
#include "TileColour.h"

void AStrategyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		Subsystem->AddMappingContext(CameraMappingContext, 1);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "ERROR: Cant get Enhanced Input Subsystem");
	}

	PlayerCam = GetPawn();

	UTBActionBase* DefaultAction = NewObject<UPlayerActionOpen>();
	SetCurrentAction(DefaultAction);
}

void AStrategyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Default action bindings
		EnhancedInputComponent->BindAction(DefaultLeftClickAction, ETriggerEvent::Started, this, &AStrategyPlayerController::PlayerLeftClick);
		EnhancedInputComponent->BindAction(DefaultRightClickAction, ETriggerEvent::Started, this, &AStrategyPlayerController::PlayerRightClick);

		// Camera action bindings
		EnhancedInputComponent->BindAction(CameraMouseMoveAction, ETriggerEvent::Triggered, this, &AStrategyPlayerController::MoveMouse);
		
		EnhancedInputComponent->BindAction(CameraRightClickAction, ETriggerEvent::Triggered, this, &AStrategyPlayerController::EnableCameraRotation);
		EnhancedInputComponent->BindAction(CameraRightClickAction, ETriggerEvent::Completed, this, &AStrategyPlayerController::DisabledCameraRotation);
		EnhancedInputComponent->BindAction(CameraRightClickAction, ETriggerEvent::Canceled, this, &AStrategyPlayerController::DisabledCameraRotation);
		
		EnhancedInputComponent->BindAction(CameraMiddleClickAction, ETriggerEvent::Triggered, this, &AStrategyPlayerController::EnableCameraPan);
		EnhancedInputComponent->BindAction(CameraMiddleClickAction, ETriggerEvent::Completed, this, &AStrategyPlayerController::DisableCameraPan);
		EnhancedInputComponent->BindAction(CameraMiddleClickAction, ETriggerEvent::Canceled, this, &AStrategyPlayerController::DisableCameraPan);
		
		EnhancedInputComponent->BindAction(CameraMiddleScrollAction, ETriggerEvent::Triggered, this, &AStrategyPlayerController::CameraZoom);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "ERROR: Cast to EIC Failed");
	}
}

void AStrategyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetMousePosition(ScreenMousePosition.X, ScreenMousePosition.Y);
	DeprojectMousePositionToWorld(WorldMousePosition, WorldMouseDirection);

	FHitResult HitResult;
	if(GetHitResultUnderCursor(ECC_Visibility, true, HitResult))
	{
		if (AActor* HitTile = UTileMapFunctionLibrary::GetBelowTile(HitResult.Location, GetWorld()))
		{
			if(UTileComponent* NewTileComponent = IIsTile::Execute_GetTileComponent(HitTile))
			{
				CurrentAction->OnHover(CurrentHoveredTileComponent, NewTileComponent);
				
				CurrentHoveredTileComponent = NewTileComponent;
			}
		}
	}
}

void AStrategyPlayerController::DisableHovering()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("DISABLE HOVER")));
	
	CurrentAction->DisableHover(CurrentHoveredTileComponent);
	CurrentHoveredTileComponent = nullptr;
}

void AStrategyPlayerController::EnableHovering()
{
	CurrentAction->EnableHover();
}

void AStrategyPlayerController::SetCurrentAction(UTBActionBase* NewAction)
{
	CurrentAction = NewAction;
	CurrentAction->GetActionEndDelegate()->AddUObject(this, &AStrategyPlayerController::EndAction_Implementation);
}

void AStrategyPlayerController::EndAction_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Purple, FString::Printf(TEXT("Ended action - set default action")));

	UTBActionBase* DefaultAction = NewObject<UPlayerActionOpen>();
	SetCurrentAction(DefaultAction);
}

void AStrategyPlayerController::PlayerLeftClick()
{
	// If the tile we're hovering to select is not already selected
	if(CurrentHoveredTileComponent != CurrentSelectedTileComponent)
	{
		if(IsValid(CurrentSelectedTileComponent))
		{
			CurrentSelectedTileComponent->TileUnSelect();
		}
		
		CurrentSelectedTileComponent = CurrentHoveredTileComponent;

		//
		CurrentAction->OnLeftClick(CurrentSelectedTileComponent);
	}
}

void AStrategyPlayerController::PlayerRightClick()
{
	CurrentAction->OnRightClick();
}

void AStrategyPlayerController::EnableCameraRotation()
{
	bCanRotateCamera = true;
}

void AStrategyPlayerController::DisabledCameraRotation()
{
	bCanRotateCamera = false;
}

void AStrategyPlayerController::MoveMouse(const FInputActionValue& Value)
{
	FVector2D AxisVector = Value.Get<FVector2D>();

	if (bCanRotateCamera)
	{
		PlayerCam->AddControllerYawInput(AxisVector.X * 4);
		PlayerCam->AddControllerPitchInput(-AxisVector.Y * 4);
	}
	
	if (bCanPanCamera)
	{
		PlayerCam->AddActorLocalOffset(FVector(0, -AxisVector.X * 200, -AxisVector.Y * 200));
	}
}

void AStrategyPlayerController::EnableCameraPan()
{
	bCanPanCamera = true;
}

void AStrategyPlayerController::DisableCameraPan()
{
	bCanPanCamera = false;
}

void AStrategyPlayerController::CameraZoom(const FInputActionValue& Value)
{
	float ScrollAxisValue = Value.Get<float>();

	FVector ZoomVector = WorldMouseDirection * 500.f;
	ZoomVector *= 0.2;
	PlayerCam->AddActorWorldOffset(ZoomVector * ScrollAxisValue);
}

void AStrategyPlayerController::FindPathFromSelected()
{
	if (IsValid(CurrentSelectedTileComponent))
	{
		if (PlayerCam->Implements<UHasPathfinding>())
		{
			if (UPathfindingComponent* PathfinderComponent = IHasPathfinding::Execute_GetPathfindingComponent(PlayerCam))
			{
				//PathfinderComponent->HighlightTiles(PathfinderComponent->AttemptPathfinding(CurrentSelectedTileComponent, CurrentHoveredTileComponent));
				//PathfinderComponent->HighlightTilesInRange(CurrentSelectedTileComponent, PathfinderComponent->GetMoveDistance());
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("ERROR: Couldnt get pathfinding component...")));
			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("ERROR: Player doesnt implement pathfinding interface...")));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("ERROR: Selected tile owner invalid...")));
	}
}
