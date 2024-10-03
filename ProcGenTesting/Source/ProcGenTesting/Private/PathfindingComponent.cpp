// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingComponent.h"

#include "TileComponent.h"
#include "MapGeneratorComponent.h"
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

// TODO: Fix up GetSurroundingTiles function to actually get surrounding tiles
void UPathfindingComponent::AttemptPathfinding(UTileComponent* StartTile, UTileComponent* TargetTile)
{
	OpenSet.Empty();
	ClosedSet.Empty();
	
	if(TargetTile->bIsWalkable && !TargetTile->bIsOccupied)
	{
		StartTile->GCost = 0;
		OpenSet.Add(StartTile);

		UMapGeneratorComponent* MapGenerator = StartTile->OwningRoom->MapGenerator;
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
				RetracePath(TargetTile, StartTile);
			}

			for(UTileComponent* Neighbour : CurrentTile->SurroundingTiles)
			{
				if(!Neighbour->bIsWalkable or ClosedSet.Contains(Neighbour))
				{
					continue;
				}
				
				int NewNeighbourGCost = CurrentTile->GCost + GetDistance(CurrentTile, Neighbour);
				if(NewNeighbourGCost < Neighbour->GCost or !OpenSet.Contains(Neighbour))
				{
					Neighbour->GCost = NewNeighbourGCost;
					Neighbour->HCost = GetDistance(Neighbour, TargetTile);
					Neighbour->FCost = Neighbour->GCost + Neighbour->HCost;
					Neighbour->ParentTile = CurrentTile;

					if(!OpenSet.Contains(Neighbour))
					{
						OpenSet.Add(Neighbour);
					}
				}
			}
		}
	}
}

void UPathfindingComponent::RetracePath(UTileComponent* TargetNode, UTileComponent* StartNode)
{
	Path.Empty();
	UTileComponent* CurrentTile = StartNode;

	while(CurrentTile != TargetNode)
	{
		Path.Add(CurrentTile);
		CurrentTile = CurrentTile->ParentTile;
	}

	Algo::Reverse(Path);
	for(UTileComponent* Tile : Path)
	{
		HighlightPath();
	}
}

void UPathfindingComponent::HighlightPath()
{
	for(UTileComponent* TileComp : Path)
	{
		// Change colour of tile
		if(ATileNode* Tile = Cast<ATileNode>(TileComp->GetOwner()))
		{
			Tile->AddToOverlayColour(FLinearColor::Green);
		}
	}
}

int UPathfindingComponent::GetDistance(UTileComponent* TileA, UTileComponent* TileB)
{
	int DistanceX = abs(TileA->GetOwner()->GetActorLocation().X - TileB->GetOwner()->GetActorLocation().X);
	int DistanceY = abs(TileA->GetOwner()->GetActorLocation().Y - TileB->GetOwner()->GetActorLocation().Y);

	if(DistanceX > DistanceY)
	{
		return (1.4f * TileA->GetOwningRoom()->TileSize) * DistanceY + (1.0f * TileA->GetOwningRoom()->TileSize) * DistanceX - DistanceY;
	}

	return (1.4 * TileA->GetOwningRoom()->TileSize) * DistanceX + (1.f * TileA->GetOwningRoom()->TileSize) * (DistanceY - DistanceX);
}
