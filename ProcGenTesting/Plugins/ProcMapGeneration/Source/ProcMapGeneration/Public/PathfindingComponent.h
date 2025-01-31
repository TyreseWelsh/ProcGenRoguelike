// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingComponent.generated.h"

DECLARE_DELEGATE(FOnMovementEndSignature);


class UTileComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCMAPGENERATION_API UPathfindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfindingComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Init();
	UFUNCTION()
	virtual void Move();
	virtual bool EndMove();

	TArray<AActor*> AttemptPathfinding(UTileComponent* StartTile, UTileComponent* TargetTile);
	void HighlightTilesInNewPath(UTileComponent* StartTile, UTileComponent* TargetTile, FLinearColor HighlightColour);
	void HighlightTilesInRange(UTileComponent* StartTile, int Range, FLinearColor HighlightColour);
	
	void HighlightTiles(TArray<AActor*> TilesToHighlight, FLinearColor HighlightColour);
	void UnHighlightTiles(TArray<AActor*> TilesToUnHighlight, FLinearColor UnHighlightColour);

	TArray<AActor*> FindTilesInRange(UTileComponent* StartTile, int Range);
	TArray<AActor*> FindValidTiles();
	
	int GetMoveDistance() const { return MoveDistance; }
	TArray<AActor*> GetCurrentPath() const { return Path; }
	TArray<AActor*> GetTilesInRange() const { return TilesInRange; }
	TArray<AActor*> GetValidTiles() const { return ValidTiles; }

	FTimerHandle GetMoveTimer() const { return MoveTimerHandle; }

	FOnMovementEndSignature* GetMovementEndDelegate() { return &MovementEndDelegate; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY()
	FTimerHandle HighlightPathTimerHandle;
	FTimerDelegate HighlightPathTimerDelegate;

	UPROPERTY()
	FTimerHandle MoveTimerHandle;
	FTimerDelegate MoveTimerDelegate;

	FOnMovementEndSignature MovementEndDelegate;
	
	UPROPERTY()
	TArray<AActor*> Path;
	
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pathfinding", meta=(AllowPrivateAccess=true))
	int MoveDistance;
	UPROPERTY()
	TArray<AActor*> TilesInRange;
	UPROPERTY()
	TArray<AActor*> ValidTiles;
	
private:
	TArray<AActor*> RetracePath(UTileComponent* StartNode, UTileComponent* TargetNode);
	int GetDistance(UTileComponent* TileA, UTileComponent* TileB) const;
	int GetDistanceX(UTileComponent* TileA, UTileComponent* TileB) const;
	int GetDistanceY(UTileComponent* TileA, UTileComponent* TileB) const;
	
	TArray<UTileComponent*> OpenSet;
	TArray<UTileComponent*> ClosedSet;


};
