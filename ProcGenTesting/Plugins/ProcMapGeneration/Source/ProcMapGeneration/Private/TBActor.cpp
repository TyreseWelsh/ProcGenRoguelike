// Fill out your copyright notice in the Description page of Project Settings.


#include "TBActor.h"

// Sets default values
ATBActor::ATBActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATBActor::StartTurn()
{
}

void ATBActor::EndTurn()
{
}

