// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "HasPathfinding.h"
#include "PathfindingComponent.h"
#include "TileMapFunctionLibrary.h"
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
	
	/*FVector WorldMousePos;
	FVector MouseDirection;
	DeprojectMousePositionToWorld(WorldMousePos, MouseDirection);

	//FVector2D ScreenMousPos;
	//GetMousePosition(ScreenMousPos.X, ScreenMousPos.Y);

	FVector PlayerPosition = PlayerCam->GetActorLocation();

	// TODO: For pythagoras, the shortest side should be PlayerPos - MapGenerator.Origin : Bottom side is abs(MousePosition - PlayerPos)
	float PythagA = PlayerCam->GetActorLocation().Z;
	FVector PythagBVector = FVector(WorldMousePos.X, WorldMousePos.Y, 0.f) - FVector(PlayerCam->GetActorLocation().X, PlayerCam->GetActorLocation().Y, 0.0f);
	float PythagB = PythagBVector.Length();
	float RayLength = sqrtf(PythagA * PythagA + PythagB * PythagB);
	RayLength *= 1.25f;
	
	//WorldMousePos.Z = PlayerPosition.Z;
	
	FHitResult HitResult;
	FVector TraceStart = FVector(WorldMousePos.X, WorldMousePos.Y, WorldMousePos.Z - 100.f);
	FVector TraceEnd = FVector(WorldMousePos.X, WorldMousePos.Y, WorldMousePos.Z - 5000.f);

	FVector NewTraceStart = TraceStart + MouseDirection * RayLength;

	FHitResult HitResult2;*/
	//GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceStart + MouseDirection * RayLength, ECC_Visibility);
	//GetWorld()->LineTraceSingleByChannel(HitResult2, HitResult.TraceEnd, FVector(HitResult.TraceEnd.X, HitResult.TraceEnd.Y, HitResult.TraceEnd.Z + 3000), ECC_Visibility);

	//DrawDebugLine(GetWorld(), TraceStart, TraceStart + MouseDirection * RayLength, FColor::Cyan, false, 100, 0, 2.f);
	//DrawDebugLine(GetWorld(), HitResult.TraceEnd, FVector(HitResult.TraceEnd.X, HitResult.TraceEnd.Y, HitResult.TraceEnd.Z + 3000), FColor::Red, false, 20, 0, 2.f);

	//DrawDebugLine(GetWorld(), FVector(PlayerCam->GetActorLocation().X, PlayerCam->GetActorLocation().Y, 0.0f), FVector(WorldMousePos.X, WorldMousePos.Y, 0.f), FColor::Black, false, 20, 0, 2.f);
	// DrawDebugLine(GetWorld(), WorldMousePos, FVector(WorldMousePos.X, WorldMousePos.Y, WorldMousePos.Z - 3000), FColor::Red, false, 20, 0, 1.f);

	//////bool HitSuccess = GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	
	//WorldMousePos.Z = 0;
	//DrawDebugLine(GetWorld(), WorldMousePos, FVector(WorldMousePos.X, WorldMousePos.Y, WorldMousePos.Z + 200.f), FColor::Yellow, false, 100, 0, 2);
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%f"), WorldMousePos.Z));

	
	//GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	//TArray<FHitResult> Hits;
	//GetWorld()->LineTraceMultiByChannel(Hits, PlayerPosition, TraceStart + MouseDirection * RayLength, ECC_Visibility);
	
	/*if(IsValid(HitResult.GetActor()))
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
	}*/


	FHitResult HitResult;
	bool HitSuccess = GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	if(HitSuccess)
	{
		if (AActor* HitTile = UTileMapFunctionLibrary::GetBelowTile(HitResult.Location, GetWorld()))
		{
			if(HitTile->Implements<UIsTile>())
			{
				if(UTileComponent* NewTileComponent = IIsTile::Execute_GetTileComponent(HitTile))
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

	if (IsValid(CurrentSelectedTileComponent))
	{
		if (PlayerCam->Implements<UHasPathfinding>())
		{
			if (UPathfindingComponent* PathfinderComponent = IHasPathfinding::Execute_GetPathfindingComponent(PlayerCam))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Attempting pathfinding...")));

				PathfinderComponent->HighlightPath(PathfinderComponent->AttemptPathfinding(CurrentSelectedTileComponent, CurrentHoveredTileComponent));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("ERROR: Couldnt get pathfinding component...")));

			}
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("ERROR: Player doesnt implement pathfinding interface...")));

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("ERROR: Selected tile owner invalid...")));
	}
}
