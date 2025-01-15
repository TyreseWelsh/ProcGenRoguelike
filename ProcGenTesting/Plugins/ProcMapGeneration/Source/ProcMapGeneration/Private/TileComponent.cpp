// Fill out your copyright notice in the Description page of Project Settings.


#include "TileComponent.h"
#include "MapGeneratorComponent.h"
#include "MapRoom.h"
#include "ExitGenerator.h"
#include "IsTile.h"
#include "TileMapFunctionLibrary.h"

// Sets default values for this component's properties
UTileComponent::UTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

void UTileComponent::InitTile_Implementation(AMapRoom* OwnerRoom, int NewTileSize, int NewRoomIndexX, int NewRoomIndexY)
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
	
	//FindSurroundingTiles();
	SetTileType();
}

void UTileComponent::SetTileType()
{
	SetTileTypeToGround();

	/*if(FMath::RandRange(1, 40) == 1)
	{
		SetTileTypeToWall();
	}*/

	/*UMapGeneratorComponent* MapGenerator = OwningRoom->MapGenerator;
	int MapIndex1D = UTileMapFunctionLibrary::CalculateIndexFromTilePos(GetOwner()->GetActorLocation());
	FVector2D MapIndex2D = UTileMapFunctionLibrary::ConvertIndex1Dto2D(MapIndex1D);

	//UE_LOG(LogTemp, Display, TEXT("%i"), MapGenerator->MapSizeX);
	if(MapIndex2D.X == 0 or MapIndex2D.X * TileSize == MapGenerator->MapSizeX - TileSize or MapIndex2D.Y == 0 or MapIndex2D.Y * TileSize == MapGenerator->MapSizeY - TileSize)
	{
		// Set tile type as wall
		SetTileTypeToWall();
		return;
	}*/
	/*if(RoomIndexX == 1 or RoomIndexX == OwningRoom->LastIndexX - 1 or RoomIndexY == 1 or RoomIndexY == OwningRoom->LastIndexY - 1)
	{
		if(FMath::RandRange(1, 10) <= 5)
		{
			// Set tile type as wall
			SetTileTypeToWall();
		}
	}*/

	//CheckSurroundedByWalls();
	
	TArray<AActor*> NearbyExits;
	GetOverlappingActors(NearbyExits, AExitGenerator::StaticClass());
	if(NearbyExits.Num() > 0)
	{
		SetTileTypeToPath();
	}
}

void UTileComponent::FindNeighbourTiles()
{
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			if(x == 0 && y == 0 
				or x == -1 && y == -1 
				or x == 1 && y == -1 
				or x == -1 && y == 1 
				or x == 1 && y == 1)
			{
				continue;
			}

			FVector NeighbourTilePos = FVector(GetOwner()->GetActorLocation().X + (x * TileSize), GetOwner()->GetActorLocation().Y + (y * TileSize), GetOwner()->GetActorLocation().Z);
			
			if(AActor* NeighbourTile = UTileMapFunctionLibrary::GetBelowTile(NeighbourTilePos, GetOwner()->GetWorld()))
			{
				if(UTileComponent* NeighbourTileComponent = IIsTile::Execute_GetTileComponent(NeighbourTile))
				{
					NeighbourTiles.AddUnique(NeighbourTileComponent);
				}
			}
		}
	}
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

TArray<UTileComponent*> UTileComponent::GetNeighbourTiles()
{
	/*if(NeighbourTiles.Num() > 0)
	{
		return NeighbourTiles;
	}*/

	FindNeighbourTiles();
	return NeighbourTiles;
}

void UTileComponent::SetOwningRoom(AMapRoom* NewOwner)
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

void UTileComponent::TileHover()
{
	if(HoverDelegate.IsBound())
	{
		HoverDelegate.Broadcast();
	}
}

void UTileComponent::TileHoverSelected()
{
	/*if(HoverSelectedDelegate.IsBound())
	{
		HoverSelectedDelegate.Broadcast();
	}*/
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

/*
void UTileComponent::FindSurroundingTiles()
{
	TObjectPtr<UMapGeneratorComponent> Generator = OwningRoom->MapGenerator;

	// Left tile
	int LeftTileIndex = Generator->CalculateMapIndexFromTilePos(GetOwner()->GetActorLocation() - FVector(TileSize, 0, 0));
	if(LeftTileIndex >= 0 && LeftTileIndex < Generator->MapTiles.Num())
	{
		if(AActor* TileActor = Generator->MapTiles[LeftTileIndex])
		{
			LeftTile = TileActor;
			SurroundingTiles.Add(LeftTile);

			TObjectPtr<UTileComponent> LeftTileComponent = LeftTile->FindComponentByClass<UTileComponent>();
			LeftTileComponent->RightTile = GetOwner();
			LeftTileComponent->SurroundingTiles.Add(GetOwner());
		}
	}

	// Top tile
	int TopTileIndex = Generator->CalculateMapIndexFromTilePos(GetOwner()->GetActorLocation() - FVector(0, TileSize, 0));
	if(TopTileIndex >= 0 && TopTileIndex < Generator->MapTiles.Num())
	{
		if(AActor* TileActor = Generator->MapTiles[TopTileIndex])
		{
			TopTile = TileActor;
			SurroundingTiles.Add(TopTile);
			
			TObjectPtr<UTileComponent> TopTileComponent = TopTile->FindComponentByClass<UTileComponent>();
			TopTileComponent->BottomTile = GetOwner();
			TopTileComponent->SurroundingTiles.Add(GetOwner());
		}
	}
}

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
