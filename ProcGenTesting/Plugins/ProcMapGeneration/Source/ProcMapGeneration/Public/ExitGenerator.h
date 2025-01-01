// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TeleportPoint.h"
#include "GameFramework/Actor.h"
#include "ExitGenerator.generated.h"

class USceneComponent;
class UCapsuleComponent;
class ATeleportPoint;

UCLASS()
class PROCMAPGENERATION_API AExitGenerator : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Origin;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* PathGenCollider;;
	
public:	
	// Sets default values for this actor's properties
	AExitGenerator();
	/*~AExitGenerator()
	{
		LeftTeleportPoint->Destroy();
		RightTeleportPoint->Destroy();
	}*/

	void Init(int _TileSize);
	void CalculateRelativeExits();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Public Properties
	FVector LeftTeleporterPos = FVector::ZeroVector;
	FVector RightTeleporterPos = FVector::ZeroVector;
	
	AActor* GetLeftTile() { return LeftTile; }
	AActor* GetRightTile() { return RightTile; }
	
	ATeleportPoint* GetLeftTeleportPoint() { return LeftTeleportPoint; }
	ATeleportPoint* GetRightTeleportPoint() { return RightTeleportPoint; }

private:
	void InitPathGenCollider();
	

	// Private properties
	int TileSize = 0;
	float PathLength = 0.f;
	float PathOffset = 0.f;

	int MinPathLength = 2;
	int MaxPathLength = 3;

	UPROPERTY()
	TObjectPtr<ATeleportPoint> LeftTeleportPoint;
	UPROPERTY()
	TObjectPtr<ATeleportPoint> RightTeleportPoint;
	
	UPROPERTY()
	TObjectPtr<AActor> LeftTile;
	UPROPERTY()
	TObjectPtr<AActor> RightTile;
};
