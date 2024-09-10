// Fill out your copyright notice in the Description page of Project Settings.


#include "TileComponent.h"
#include "MapRoom.h"

// Sets default values for this component's properties
UTileComponent::UTileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTileComponent::InitTile_Implementation(UMapRoom* OwnerRoom, int NewRoomIndexX, int NewRoomIndexY)
{
	OwningRoom = OwnerRoom;
	RoomIndexX = NewRoomIndexX;
	RoomIndexY = NewRoomIndexY;

	SetTileType();
}

void UTileComponent::SetTileType()
{
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
			return;
		}
	}
	//
	//if()
	
	//
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