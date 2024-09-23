// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ProcMapGeneration/Public/TileComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

void AStrategyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "ERROR: Cant get Enhanced Input Subsystem");
	}

	/*UGameplayStatics::GetGameMode(GetWorld())->Pawn*/
	PlayerCam = GetPawn();
}

void AStrategyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AStrategyPlayerController::PlayerLeftClick);
		EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AStrategyPlayerController::PlayerRightClick);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "ERROR: Cast to EIC Failed");
	}
}

void AStrategyPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector MousePos;
	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);

	FHitResult HitResult;
	FVector EndLocation = FVector(MousePos.X, MousePos.Y, PlayerCam->GetActorLocation().Z - 5000.f);
	GetWorld()->LineTraceSingleByChannel(HitResult, MousePos, EndLocation, ECC_Visibility);

	if(IsValid(HitResult.GetActor()))
	{
		if(UTileComponent* NewTileComponent = HitResult.GetActor()->FindComponentByClass<UTileComponent>())
		{
			if(NewTileComponent != CurrentHoveredTileComponent)
			{
				// Unhovering previous hovered tile first
				if(IsValid(CurrentHoveredTileComponent))
				{
					CurrentHoveredTileComponent->TileUnHover();
				}

				CurrentHoveredTileComponent = NewTileComponent;
				CurrentHoveredTileComponent->TileHover();
			}
		}
	}
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
		CurrentSelectedTileComponent->TileLeftClick();
	}
}

void AStrategyPlayerController::PlayerRightClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, "Right Click...");
}
