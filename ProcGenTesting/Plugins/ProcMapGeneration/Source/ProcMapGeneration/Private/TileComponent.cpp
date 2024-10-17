// Fill out your copyright notice in the Description page of Project Settings.


#include "TileComponent.h"
#include "MapGeneratorComponent.h"
#include "MapRoom.h"
#include "ExitGenerator.h"
#include "TileMapFunctionLibrary.h"

// Sets default values for this component's properties
UTileComponent::UTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

void UTileComponent::InitTile_Implementation(UMapRoom* OwnerRoom, int NewTileSize, int NewRoomIndexX, int NewRoomIndexY)
{
	OwningRoom = OwnerRoom;
	TileSize = NewTileSize;
	RoomIndexX = NewRoomIndexX;
	RoomIndexY = NewRoomIndexY;
	TileHeight = OwnerRoom->MapGenerator->CalculateTileHeight(GetOwner()->GetActorLocation().X, GetOwner()->GetActorLocation().Y);
	
	int BoxSize = TileSize / 2;
	
	SetRelativeLocation(FVector(0.f, 0.f, BoxSize));
	SetBoxExtent(FVector(BoxSize, BoxSize, BoxSize));
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	
	SetTileType();
}

void UTileComponent::SetTileType()
{
	SetTileTypeToGround();
}

// Called when the game starts
void UTileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UTileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTileComponent::SetOwningRoom(UMapRoom* NewOwner)
{
	OwningRoom = NewOwner;
}

void UTileComponent::SetRoomIndexX(int NewIndex)
{
	RoomIndexX = NewIndex;
}

void UTileComponent::SetRoomIndexY(int NewIndex)
{
	RoomIndexY = NewIndex;
}

void UTileComponent::SetTeleportTarget(AActor* NewTarget)
{
	TeleportTarget = NewTarget;
}

void UTileComponent::TileHover()
{
	if(HoverDelegate.IsBound())
	{
		HoverDelegate.Broadcast();
	}
}

void UTileComponent::TileHoverSelected()
{
	if(HoverSelectedDelegate.IsBound())
	{
		HoverSelectedDelegate.Broadcast();
	}
}

void UTileComponent::TileUnHover()
{
	if(UnHoverDelegate.IsBound())
	{
		UnHoverDelegate.Broadcast();
	}
}

void UTileComponent::TileLeftClick()
{
	if(LeftClickDelegate.IsBound())
	{
		LeftClickDelegate.Broadcast();
	}
}

void UTileComponent::TileRightClick()
{
	if(RightClickDelegate.IsBound())
	{
		RightClickDelegate.Broadcast();
	}
}

void UTileComponent::TileUnSelect()
{
	if(UnSelectDelegate.IsBound())
	{
		UnSelectDelegate.Broadcast();
	}
}

void UTileComponent::SetTileTypeToWall()
{
	TileType = ETileType::Wall;
	if(TileTypeToWallDelegate.IsBound())
	{
		TileTypeToWallDelegate.Broadcast();
	}
}

void UTileComponent::SetTileTypeToPath()
{
	TileType = ETileType::Path;
	if(TileTypeToPathDelegate.IsBound())
	{
		TileTypeToPathDelegate.Broadcast();
	}
}

void UTileComponent::SetTileTypeToGround()
{
	TileType = ETileType::Ground;
	if(TileTypeToGroundDelegate.IsBound())
	{
		TileTypeToGroundDelegate.Broadcast();
	}
}

void UTileComponent::SetTileTypeToExit()
{
	TileType = ETileType::Exit;
	if(TileTypeToExitDelegate.IsBound())
	{
		TileTypeToExitDelegate.Broadcast();
	}
}

void UTileComponent::SetOccupyingActor(AActor* NewOccupyingActor)
{
	OccupyingActor = NewOccupyingActor;
}

TArray<AActor*> UTileComponent::FindNeighbourTiles()
{
	TArray<AActor*> NeighbourTiles;
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			if (x == 0 && y == 0)
				continue;

			FVector NeighbourTilePosition = FVector(OwnerLocation.X + (TileSize * x), OwnerLocation.Y + (TileSize * y), OwnerLocation.Z);
			if (AActor* NeighbourTile = UTileMapFunctionLibrary::GetBelowTile(NeighbourTilePosition, GetOwner()->GetWorld()))
			{
				NeighbourTiles.Add(NeighbourTile);
			}
		}
	}
	return NeighbourTiles;
}


/*
void UTileComponent::CheckSurroundedByWalls()
{
	int SurroundingWallsCounter = 0;
	for(AActor* Tile : SurroundingTiles)
	{
		if(IsValid(Tile))
		{
			if(Tile->FindComponentByClass<UTileComponent>()->TileType == ETileType::Wall)
			{
				SurroundingWallsCounter++;
			}
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Invalid wall"));

			SurroundingWallsCounter++;
		}
	}
	if(SurroundingWallsCounter == SurroundingTiles.Num())
	{
		SetTileTypeToWall();
		//UE_LOG(LogTemp, Warning, TEXT("Surrounded by walls"));
	}
}
*/

