// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingComponent.h"

#include "TileComponent.h"
#include "MapRoom.h"
#include "TileNode.h"

#include "Algo/Reverse.h"

// Sets default values for this component's properties
UPathfindingComponent::UPathfindingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPathfindingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPathfindingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<AActor*> UPathfindingComponent::AttemptPathfinding(UTileComponent* StartTile, UTileComponent* TargetTile)
{
	// Remove path highlight from old path if present
	for (AActor* PathTile : Path)
	{
		if (PathTile->Implements<UIsTile>())
		{
			IIsTile::Execute_SubtractTileColour(PathTile, FLinearColor::Yellow);
		}
	}
	Path.Empty();
	
	OpenSet.Empty();
	ClosedSet.Empty();
	
	if(TargetTile->bIsWalkable && !TargetTile->bIsOccupied)
	{
		StartTile->GCost = 0;
		OpenSet.Add(StartTile);

		while(OpenSet.Num() > 0)
		{
			// Find path
			UTileComponent* CurrentTile = OpenSet[0];
			for(int i = 1; i < OpenSet.Num(); i++)
			{
				if(OpenSet[i]->FCost < CurrentTile->FCost or
					(OpenSet[i]->FCost == CurrentTile->FCost && OpenSet[i]->HCost < CurrentTile->HCost))
				{
					CurrentTile = OpenSet[i];
				}
			}
			OpenSet.Remove(CurrentTile);
			ClosedSet.Add(CurrentTile);

			if(CurrentTile == TargetTile)
			{
				// End pathfinding and calculate path
				return RetracePath(TargetTile, StartTile);
			}

			// Add valid neighbour tiles to open set
			for(AActor* NeighbourTile : CurrentTile->FindNeighbourTiles())
			{
				if (NeighbourTile->Implements<UIsTile>())
				{
					if (UTileComponent* NeighbourTileComponent = IIsTile::Execute_GetTileComponent(NeighbourTile))
					{
						if(!NeighbourTileComponent->bIsWalkable or ClosedSet.Contains(NeighbourTileComponent))
						{
							continue;
						}
				
						int NewNeighbourGCost = abs(CurrentTile->GCost + GetDistance(CurrentTile, NeighbourTileComponent));
						if(NewNeighbourGCost < NeighbourTileComponent->GCost or !OpenSet.Contains(NeighbourTileComponent))
						{
							NeighbourTileComponent->GCost = NewNeighbourGCost;
							NeighbourTileComponent->HCost = abs(GetDistance(NeighbourTileComponent, TargetTile));

							NeighbourTileComponent->FCost = NeighbourTileComponent->GCost + NeighbourTileComponent->HCost;
							NeighbourTileComponent->ParentTile = CurrentTile;

							if(!OpenSet.Contains(NeighbourTileComponent))
							{
								OpenSet.Add(NeighbourTileComponent);
							}
						}
					}
				}
			}
		}

		// 
		return Path;
	}

	// Invalid target tile (Either occupied or unwalkable)
	return Path;
}

void UPathfindingComponent::HighlightTilesInRange(UTileComponent* StartTile, int Range)
{
	UnHighlightTiles(TilesInRange);
	HighlightTiles(GetTilesInRange(StartTile, Range));
}

TArray<AActor*> UPathfindingComponent::RetracePath(UTileComponent* TargetNode, UTileComponent* StartNode)
{
	// Starting from the target tile, add tiles and their parent tile to the path array, then reverse it
	UTileComponent* CurrentTile = TargetNode;

	while(CurrentTile != StartNode)
	{
		Path.Add(CurrentTile->GetOwner());
		CurrentTile = CurrentTile->ParentTile;
	}
	//Path.Add(StartNode->GetOwner());
	Algo::Reverse(Path);

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Found path of %i tiles!"), Path.Num()));
	
	return Path;
}

TArray<AActor*> UPathfindingComponent::GetTilesInRange(UTileComponent* StartTile, int Range)
{
	TilesInRange.Empty();
	
	TArray<UTileComponent*> rangeOpenSet;
	TArray<UTileComponent*> rangeClosedSet;

	rangeOpenSet.Add(StartTile);

	while(rangeOpenSet.Num() > 0)
	{		
		UTileComponent* CurrentTileComponent = rangeOpenSet[0];
		rangeOpenSet.Remove(CurrentTileComponent);
		rangeClosedSet.Add(CurrentTileComponent);
		
		for(AActor* Tile : CurrentTileComponent->FindNeighbourTiles())
		{
			if(UTileComponent* NeighbourTileComponent = IIsTile::Execute_GetTileComponent(Tile))
			{
				if(rangeClosedSet.Contains(NeighbourTileComponent) or
					GetDistanceX(StartTile, NeighbourTileComponent) + GetDistanceY(StartTile, NeighbourTileComponent) > Range * NeighbourTileComponent->GetOwningRoom()->TileSize)
				{
					continue;
				}
				
				if(!rangeOpenSet.Contains(NeighbourTileComponent))
				{
					rangeOpenSet.Add(NeighbourTileComponent);
					TilesInRange.AddUnique(NeighbourTileComponent->GetOwner());
				}
			}
		}
	}
	
	return TilesInRange;
}

void UPathfindingComponent::HighlightTiles(TArray<AActor*> TilesToHighlight)
{
	for (AActor* Tile : TilesToHighlight)
	{
		if (Tile->Implements<UIsTile>())
		{
			IIsTile::Execute_AddTileColour(Tile, FLinearColor::Yellow);
		}
	}
}

void UPathfindingComponent::UnHighlightTiles(TArray<AActor*> TilesToUnHighlight)
{
	for (AActor* Tile : TilesToUnHighlight)
	{
		if (Tile->Implements<UIsTile>())
		{
			IIsTile::Execute_SubtractTileColour(Tile, FLinearColor::Yellow);
		}
	}
}

int UPathfindingComponent::GetDistance(UTileComponent* TileA, UTileComponent* TileB) const
{
	int DistanceX = abs(TileA->GetOwner()->GetActorLocation().X - TileB->GetOwner()->GetActorLocation().X);
	int DistanceY = abs(TileA->GetOwner()->GetActorLocation().Y - TileB->GetOwner()->GetActorLocation().Y);

	if(DistanceX > DistanceY)
	{
		return (14.f * TileA->GetOwningRoom()->TileSize) * DistanceY + (10.f * TileA->GetOwningRoom()->TileSize) * (DistanceX - DistanceY);
	}

	return (14.f * TileA->GetOwningRoom()->TileSize) * DistanceX + (10.f * TileA->GetOwningRoom()->TileSize) * (DistanceY - DistanceX);
}

int UPathfindingComponent::GetDistanceX(UTileComponent* TileA, UTileComponent* TileB) const
{
	return abs(TileA->GetOwner()->GetActorLocation().X - TileB->GetOwner()->GetActorLocation().X);
}

int UPathfindingComponent::GetDistanceY(UTileComponent* TileA, UTileComponent* TileB) const
{
	return abs(TileA->GetOwner()->GetActorLocation().Y - TileB->GetOwner()->GetActorLocation().Y);
}
