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
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString(TEXT("Reached...")));

		if(TargetIndex + 1 < ValidTiles.Num())
		{
			TargetIndex++;
		}
		else
		{
			// End movement
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString(TEXT("End Movement...")));
			MovementEndDelegate.ExecuteIfBound();
			GetOwner()->GetWorldTimerManager().ClearTimer(MoveTimerHandle);
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

void UPlayerPathfindingComponent::Move()
{
	if(!ValidTiles.IsEmpty())
	{
		TargetIndex = 0;
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, MoveTimerDelegate, 0.02f, true);
		
		/*AActor* TargetTile = ValidTiles.Top();
		GetOwner()->SetActorLocation(FVector(TargetTile->GetActorLocation().X, TargetTile->GetActorLocation().Y, 0));*/
		/*for(int i = 0; i < ValidTiles.Num(); ++i)
		{
			float MoveTime = GetOwner()->GetWorld()->GetTimeSeconds();
			
			float DistanceAlpha = 0;
			
			FVector StartPos = GetOwner()->GetActorLocation();
			FVector TargetPos = FVector(ValidTiles[i]->GetActorLocation().X, ValidTiles[i]->GetActorLocation().Y, StartPos.Z);
			while(FVector::Distance(GetOwner()->GetActorLocation(), TargetPos) > 2)
			{

				DistanceAlpha += GetOwner()->GetWorld()->DeltaTimeSeconds * 0.0001f;
				//GEngine->AddOnScreenDebugMessage(-1, 150.f, FColor::Red, FString::Printf(TEXT("Alpha: %f"), DistanceAlpha));

				FVector CurrentPos = FMath::InterpEaseInOut<FVector>(StartPos, TargetPos, DistanceAlpha, 90.f);
				//FVector CurrentPo = FMath::Lerp(StartPos, TargetPos, DistanceAlpha);
				GetOwner()->SetActorLocation(CurrentPos);
			}

			GetOwner()->SetActorLocation(TargetPos);
			
			MoveTime -= GetOwner()->GetWorld()->GetTimeSeconds();
			GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Green, FString::Printf(TEXT("NEXT TILE : %f"), MoveTime));

		}*/
	}
}
