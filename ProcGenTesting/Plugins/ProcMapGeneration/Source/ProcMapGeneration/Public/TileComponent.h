// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "TileComponent.generated.h"

UENUM()
enum ETileType
{
	Wall	UMETA(DisplayName = "Wall"),
	Path	UMETA(DisplayName = "Path"),
	Ground	UMETA(DisplayName = "Ground"),
	Exit	UMETA(DisplayName = "Exit"),
};

DECLARE_MULTICAST_DELEGATE(FOnMouseHoverSignature);
DECLARE_MULTICAST_DELEGATE(FOnMouseHoverSelectedSignature);
DECLARE_MULTICAST_DELEGATE(FOnMouseUnHoverSignature);
DECLARE_MULTICAST_DELEGATE(FOnLeftClickSignature);
DECLARE_MULTICAST_DELEGATE(FOnRightClickSignature);
DECLARE_MULTICAST_DELEGATE(FUnSelectSignature);

DECLARE_MULTICAST_DELEGATE(FTileTypeToWallSignature);
DECLARE_MULTICAST_DELEGATE(FTileTypeToPathSignature);
DECLARE_MULTICAST_DELEGATE(FTileTypeToGroundSignature);
DECLARE_MULTICAST_DELEGATE(FTileTypeToExitSignature);

class AMapRoom;

/*
 *  Component to be attached to any actor the user wishes to be treated as a "Tile"
 *  Enables tile functionality on the attached actor through the TileComponent functions provided
 *  The components functions must be implemented in the owning Actor to implement custom behaviour
 *  These functions will act as an interface to interact with the Actor and will be called by the plugin's custom Controller
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class PROCMAPGENERATION_API UTileComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitTile(AMapRoom* OwnerRoom, int NewTileSize, int NewRoomIndexX, int NewRoomIndexY);
	virtual void InitTile_Implementation(AMapRoom* OwnerRoom, int NewTileSize, int NewRoomIndexX, int NewRoomIndexY);

	void SetTileType();
	void FindNeighbourTiles();
	void CheckSurroundedByWalls();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TObjectPtr<AMapRoom> OwningRoom;
	TEnumAsByte<ETileType> TileType;

	bool bIsWalkable = true;
	bool bIsOccupied = false;
	int GCost = 0;
	int HCost = 0;
	int FCost = 0;
	UPROPERTY()
	UTileComponent* ParentTile;
	UPROPERTY()
	TArray<UTileComponent*> NeighbourTiles;

	TArray<UTileComponent*> GetNeighbourTiles();

	AActor* GetOccupyingObject() { return OccupyingObject; }
	void SetOccupyingObject(AActor* NewObject) { OccupyingObject = NewObject; }

	void SetOwningRoom(AMapRoom* NewOwner);
	void SetRoomIndexX(int NewIndex);
	void SetRoomIndexY(int NewIndex);

	void TileHover();
	void TileHoverSelected();
	void TileUnHover();
	void TileLeftClick();
	void TileRightClick();
	void TileUnSelect();
	
	void SetTileTypeToWall();
	void SetTileTypeToPath();
	void SetTileTypeToGround();
	void SetTileTypeToExit();

	TObjectPtr<AMapRoom> GetOwningRoom() const { return OwningRoom; }
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<ETileType> GetTileType() const { return TileType; }
	UFUNCTION(BlueprintCallable)
	float GetTileHeight() const { return TileHeight; }
	UFUNCTION(BlueprintCallable)
	int GetRoomIndexX() const { return RoomIndexX; }
	UFUNCTION(BlueprintCallable)
	int GetRoomIndexY() const { return RoomIndexY; }

	FOnMouseHoverSignature* GetMouseHoverDelegate() { return &HoverDelegate; }
	FOnMouseUnHoverSignature* GetMouseUnHoverDelegate() { return &UnHoverDelegate; }
	FOnLeftClickSignature* GetLeftClickDelegate() { return &LeftClickDelegate; }
	FUnSelectSignature* GetUnSelectDelegate() { return &UnSelectDelegate; }
	
	FTileTypeToWallSignature* GetTileTypeToWallDelegate() { return &TileTypeToWallDelegate; }
	FTileTypeToPathSignature* GetTileTypeToPathDelegate() { return &TileTypeToPathDelegate; }
	FTileTypeToGroundSignature* GetTileTypeToGroundDelegate() { return &TileTypeToGroundDelegate; }
	FTileTypeToExitSignature* GetTileTypeToExitDelegate() { return &TileTypeToExitDelegate; }
	

private:
	void FindSurroundingTiles();
	
	int TileSize = 0;
	float TileHeight = 0.f;
	
	int RoomIndexX = 0;
	int RoomIndexY = 0;

	UPROPERTY()
	TObjectPtr<AActor> OccupyingObject;
	
	UPROPERTY()
	TObjectPtr<AActor> LeftTile;
	UPROPERTY()
	TObjectPtr<AActor> RightTile;
	UPROPERTY()
	TObjectPtr<AActor> TopTile;
	UPROPERTY()
	TObjectPtr<AActor> BottomTile;


	// Tile Interaction Delegeates
	FOnMouseHoverSignature HoverDelegate;
	FOnMouseUnHoverSignature UnHoverDelegate;
	FOnLeftClickSignature LeftClickDelegate;
	FOnRightClickSignature RightClickDelegate;
	FUnSelectSignature UnSelectDelegate;
	
	// Tile Type Delegates
	FTileTypeToWallSignature TileTypeToWallDelegate;
	FTileTypeToPathSignature TileTypeToPathDelegate;
	FTileTypeToGroundSignature TileTypeToGroundDelegate;
	FTileTypeToExitSignature TileTypeToExitDelegate;
};
