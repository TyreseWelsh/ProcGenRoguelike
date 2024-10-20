// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathfindingComponent.generated.h"

class UTileComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCGENTESTING_API UPathfindingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPathfindingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TArray<AActor*> AttemptPathfinding(UTileComponent* StartTile, UTileComponent* TargetTile);
	void HighlightPath(TArray<AActor*> PathToHighlight);
	
private:
	TArray<AActor*> RetracePath(UTileComponent* StartNode, UTileComponent* TargetNode);
	int GetDistance(UTileComponent* TileA, UTileComponent* TileB);
	
	TArray<UTileComponent*> OpenSet;
	TArray<UTileComponent*> ClosedSet;
	UPROPERTY()
	TArray<AActor*> Path;
};
