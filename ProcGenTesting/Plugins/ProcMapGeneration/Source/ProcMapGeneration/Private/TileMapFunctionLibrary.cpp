// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMapFunctionLibrary.h"
#include "IsTile.h"
#include "TileComponent.h"
#include "Kismet/GameplayStatics.h"

float UTileMapFunctionLibrary::RoundToTileSizeMultiple(float OldValue, bool bRoundUp, int TileSize)
{
	float CurrentValue = OldValue / TileSize;
	CurrentValue = (bRoundUp) ? ceil(CurrentValue) : floor(CurrentValue);
	
	return CurrentValue * TileSize;
}

int UTileMapFunctionLibrary::CalculateIndexFromTilePos(FVector TilePos, FVector MapOrigin, int AreaSizeX, int TileSize)
{
	int x = (TilePos.X - MapOrigin.X) / TileSize;
	int y = (TilePos.Y - MapOrigin.Y) / TileSize;

	return ConvertIndex2DTo1D(FVector2D(x, y), AreaSizeX, TileSize);
}

FVector2D UTileMapFunctionLibrary::ConvertIndex1Dto2D(int index, int AreaSizeX, int TileSize)
{
	int DivisionValue = index / (AreaSizeX / TileSize);
	int Remainder = index % (AreaSizeX / TileSize);
	
	return FVector2D(Remainder, DivisionValue);
}

int UTileMapFunctionLibrary::ConvertIndex2DTo1D(FVector2D Index2D, int AreaSizeX, int TileSize)
{
	return Index2D.X + (Index2D.Y * (AreaSizeX / TileSize));
}

AActor* UTileMapFunctionLibrary::GetBelowTile(AActor* StartingActor)
{
	FHitResult HitResult;
	FVector TraceEnd = FVector(StartingActor->GetActorLocation().X, StartingActor->GetActorLocation().Y, StartingActor->GetActorLocation().Z - 1000);
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(StartingActor);
	StartingActor->GetWorld()->LineTraceSingleByChannel(HitResult, StartingActor->GetActorLocation(), TraceEnd, ECC_Visibility, CollisionParams);

	if (IsValid(HitResult.GetActor()))
	{
		if (HitResult.GetActor()->Implements<UIsTile>())
		{
			return HitResult.GetActor();
		}

		return nullptr;
	}

	return nullptr;
}

AActor* UTileMapFunctionLibrary::GetBelowTile(FVector StartingPos, UWorld* CurrentWorld)
{
	FHitResult HitResult;
	FVector TraceEnd = FVector(StartingPos.X, StartingPos.Y, StartingPos.Z - 1000);
	CurrentWorld->LineTraceSingleByChannel(HitResult, StartingPos, TraceEnd, ECC_Visibility);
	
	if (IsValid(HitResult.GetActor()))
	{
		if (HitResult.GetActor()->Implements<UIsTile>())
		{
			return HitResult.GetActor();
		}

		return nullptr;
	}

	return nullptr;
}

void UTileMapFunctionLibrary::OccupyTile(AActor* OccupyingActor)
{
	if (AActor* Tile = GetBelowTile(OccupyingActor))
	{
		IIsTile::Execute_GetTileComponent(Tile)->SetOccupyingActor(OccupyingActor);
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "No tile underneath");
	}
}
