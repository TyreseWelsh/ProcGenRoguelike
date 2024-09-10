// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitGenerator.h"

#include <string>

#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AExitGenerator::AExitGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin =CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	RootComponent = Origin;

	PathGenCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PathGenCollider"));
	PathGenCollider->SetRelativeRotation(FRotator(0, 0, -90));
	PathGenCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PathGenCollider->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	PathGenCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
	PathGenCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Overlap);
	PathGenCollider->SetHiddenInGame(false);
	PathGenCollider->SetupAttachment(Origin);
}

void AExitGenerator::Init(int _TileSize)
{
	TileSize = _TileSize;
	PathGenCollider->SetRelativeLocation(FVector(TileSize / 2, 0, 0));

	PathGenCollider->SetCapsuleRadius(TileSize / 4);
	InitPathGenCollider();
}

void AExitGenerator::CalculateRelativeExitTiles()
{
	FVector OffsetVector = GetActorRightVector() * FVector(PathGenCollider->GetRelativeLocation().X, 0, 0);
	
	LeftExitTilePos = (GetActorLocation() + OffsetVector) - GetActorRightVector() * TileSize;
	RightExitTilePos = GetActorLocation() + OffsetVector;
}

void AExitGenerator::InitPathGenCollider()
{
	int InitialLength = (FMath::RandBool()) ? MinPathLength : MinPathLength;
	PathLength = InitialLength * TileSize;
	PathGenCollider->SetCapsuleHalfHeight(PathLength);

	PathOffset = InitialLength - 2;
	int NewYPos = (PathOffset * TileSize) * FMath::RandRange(-1, 1);
	PathGenCollider->AddRelativeLocation(FVector(0, NewYPos, 0));

	CalculateRelativeExitTiles();
}

// Called when the game starts or when spawned
void AExitGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExitGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

