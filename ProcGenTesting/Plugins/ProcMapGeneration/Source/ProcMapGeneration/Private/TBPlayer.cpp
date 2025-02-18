// Fill out your copyright notice in the Description page of Project Settings.


#include "TBPlayer.h"

#include "Camera/CameraComponent.h"

// Sets default values
ATBPlayer::ATBPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	RootComponent = Origin;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATBPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

