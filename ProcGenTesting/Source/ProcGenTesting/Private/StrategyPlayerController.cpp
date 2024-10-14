// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ProcMapGeneration/Public/TileComponent.h"
#include "ProcMapGeneration/Public/IsTile.h"

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
	MousePos.Z = PlayerCam->GetActorLocation().Z;
	
	FHitResult HitResult;
	FVector TraceStart = FVector(MousePos.X, MousePos.Y, MousePos.Z - 100.f);
	FVector TraceEnd = FVector(MousePos.X, MousePos.Y, MousePos.Z - 5000.f);
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceStart + MouseDirection * 5000.f, ECC_Visibility);
	//DrawDebugLine(GetWorld(), TraceStart, TraceStart + MouseDirection * 5000.f, FColor::Cyan, false, 100, 0, 10.f);

	//GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	
	if(IsValid(HitResult.GetActor()))
	{
		if(HitResult.GetActor()->Implements<UIsTile>())
		{
			if(UTileComponent* NewTileComponent = IIsTile::Execute_GetTileComponent(HitResult.GetActor()))
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
