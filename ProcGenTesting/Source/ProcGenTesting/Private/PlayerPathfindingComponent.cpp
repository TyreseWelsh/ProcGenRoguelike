// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPathfindingComponent.h"
#include "TileNode.h"
#include "TileComponent.h"
#include "StrategyPlayerController.h"
#include "TileMapFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

// Called when the game starts
void UPlayerPathfindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	HighlightPathTimerDelegate.BindUFunction(this, "FindMovementTiles");
	MoveTimerDelegate.BindUFunction(this, "StartMove");
}

void UPlayerPathfindingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UPlayerPathfindingComponent::MoveToNextTile()
{
	FVector TargetLocation = ValidTiles[TargetIndex]->GetActorLocation();
	TargetLocation.Z = GetOwner()->GetActorLocation().Z;
	
	FVector DistanceToTarget = TargetLocation - GetOwner()->GetActorLocation();
	//DistanceToTarget *= 0.1f;
	DistanceToTarget.Normalize();
	DistanceToTarget *= 2;
	GetOwner()->SetActorLocation(GetOwner()->GetActorLocation() + DistanceToTarget);
	
	if(FVector::Distance(GetOwner()->GetActorLocation(), TargetLocation) < 2.f)
	{
		GetOwner()->SetActorLocation(TargetLocation);

		if(TargetIndex + 1 < ValidTiles.Num())
		{
			TargetIndex++;
		}
		else
		{
			// End movement
			EndMove();
		}
	}
}

void UPlayerPathfindingComponent::FindMovementTiles()
{
	if(UTileComponent* OccupyingTileComponent = IIsTile::Execute_GetTileComponent(UTileMapFunctionLibrary::GetBelowTile(GetOwner())))
	{
		if(AStrategyPlayerController* StrategyController = Cast<AStrategyPlayerController>(UGameplayStatics::GetPlayerController(GetOwner()->GetWorld(), 0)))
		{
			AttemptPathfinding(OccupyingTileComponent, StrategyController->GetCurrentHoveredTileComponent());
			HighlightTiles(FindValidTiles(), FLinearColor::Green);
		}
	}
}

void UPlayerPathfindingComponent::StartMove()
{
	if(!ValidTiles.IsEmpty())
	{
		UTileMapFunctionLibrary::UnOccupyTile(GetOwner());
		MoveToNextTile();
	}
}

bool UPlayerPathfindingComponent::EndMove()
{
	return Super::EndMove();
}

void UPlayerPathfindingComponent::Move()
{
	if(!ValidTiles.IsEmpty())
	{
		TargetIndex = 0;
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, MoveTimerDelegate, 0.02f, true);
	}
}
