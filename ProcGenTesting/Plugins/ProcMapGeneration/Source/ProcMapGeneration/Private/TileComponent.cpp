// Fill out your copyright notice in the Description page of Project Settings.


#include "TileComponent.h"
#include "MapGeneratorComponent.h"
#include "MapRoom.h"
#include "ExitGenerator.h"

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
	
	SetRelativeLocation(FVector(BoxSize, BoxSize, BoxSize));
	SetBoxExtent(FVector(BoxSize, BoxSize, BoxSize));
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldStatic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	
	FindSurroundingTiles();
	SetTileType();
}

void UTileComponent::SetTileType()
{
	SetTileTypeToGround();

	/*if(FMath::RandRange(1, 40) == 1)
	{
		SetTileTypeToWall();
	}*/

	if(RoomIndexX == 0 or RoomIndexX == OwningRoom->LastIndexX or RoomIndexY == 0 or RoomIndexY == OwningRoom->LastIndexY)
	{
		// Set tile type as wall
		SetTileTypeToWall();
		return;
	}
	if(RoomIndexX == 1 or RoomIndexX == OwningRoom->LastIndexX - 1 or RoomIndexY == 1 or RoomIndexY == OwningRoom->LastIndexY - 1)
	{
		if(FMath::RandRange(1, 10) <= 5)
		{
			// Set tile type as wall
			SetTileTypeToWall();
		}
	}

	//CheckSurroundedByWalls();
	
	TArray<AActor*> NearbyExits;
	GetOverlappingActors(NearbyExits, AExitGenerator::StaticClass());
	if(NearbyExits.Num() > 0)
	{
		SetTileTypeToPath();
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

void UTileComponent::FindSurroundingTiles()
{
	TObjectPtr<UMapGeneratorComponent> Generator = OwningRoom->MapGenerator;

	// Left tile
	int LeftTileIndex = Generator->CalculateMapIndexFromTilePos(GetOwner()->GetActorLocation() - FVector(TileSize, 0, 0));
	if(LeftTileIndex >= 0 && LeftTileIndex < Generator->MapTiles.Num())
	{
		/*if(AActor* OActor = Generator->MapTiles[LeftTileIndex])
		{
			if(UTileComponent* TComponent = OActor->FindComponentByClass<UTileComponent>())
			{
				LeftTile = TComponent;
				SurroundingTiles.Add(TopTile);

				LeftTile->RightTile = this;
				LeftTile->SurroundingTiles.Add(this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid tile component (LEFT)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Owning actor (LEFT)"));
		}*/

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
		/*if(AActor* OActor = Generator->MapTiles[TopTileIndex])
		{
			if(UTileComponent* TComponent = OActor->FindComponentByClass<UTileComponent>())
			{
				TopTile = TComponent;
				SurroundingTiles.Add(TopTile);

				TopTile->BottomTile = this;
				TopTile->SurroundingTiles.Add(this);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Invalid tile component (TOP)"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Owning actor (TOP)"));
		}*/

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
