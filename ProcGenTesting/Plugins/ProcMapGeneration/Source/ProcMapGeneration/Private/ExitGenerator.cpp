// Fill out your copyright notice in the Description page of Project Settings.


#include "ExitGenerator.h"

#include <string>

#include "Components/SceneComponent.h"
#include "Components/CapsuleComponent.h"
#include "TeleportPoint.h"

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
	PathGenCollider->SetHiddenInGame(true);
	PathGenCollider->SetupAttachment(Origin);

	
}

void AExitGenerator::Init(int _TileSize)
{
	TileSize = _TileSize;

	PathGenCollider->SetCapsuleRadius(TileSize / 4);
	//InitPathGenCollider();
	CalculateRelativeExits();
}

void AExitGenerator::CalculateRelativeExits()
{
	FVector OffsetVector = GetActorRightVector() * FVector(PathGenCollider->GetRelativeLocation().X, 0, 0);
	
	LeftTeleporterPos = (GetActorLocation() + OffsetVector) - GetActorRightVector() * TileSize;
	LeftTeleporterPos.Z += TileSize * 2;
	RightTeleporterPos = (GetActorLocation() + OffsetVector) + GetActorRightVector() * TileSize;
	RightTeleporterPos.Z += TileSize * 2;

	LeftTeleportPoint = GetWorld()->SpawnActor<ATeleportPoint>(ATeleportPoint::StaticClass(), LeftTeleporterPos, FRotator::ZeroRotator);
	RightTeleportPoint = GetWorld()->SpawnActor<ATeleportPoint>(ATeleportPoint::StaticClass(), RightTeleporterPos, FRotator::ZeroRotator);
}

void AExitGenerator::InitPathGenCollider()
{
	int InitialLength = (FMath::RandBool()) ? MinPathLength : MinPathLength;
	PathLength = InitialLength * TileSize;
	PathGenCollider->SetCapsuleHalfHeight(PathLength);

	PathOffset = InitialLength - 2;
	int NewYPos = (PathOffset * TileSize) * FMath::RandRange(-1, 1);
	PathGenCollider->AddRelativeLocation(FVector(0, NewYPos, 0));

	CalculateRelativeExits();
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

