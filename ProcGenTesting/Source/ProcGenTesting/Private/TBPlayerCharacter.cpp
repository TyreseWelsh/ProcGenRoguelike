// Fill out your copyright notice in the Description page of Project Settings.


#include "TBPlayerCharacter.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ATBPlayerCharacter::ATBPlayerCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MainCollider"));
	RootComponent = MainCollider;

	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempMesh->SetupAttachment(MainCollider);

	FrontFacingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FrontFacingArrow"));
	FrontFacingArrow->SetupAttachment(TempMesh);
	FrontFacingArrow->SetHiddenInGame(false);
}

// Called when the game starts or when spawned
void ATBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UPathfindingComponent* ATBPlayerCharacter::GetPathfindingComponent_Implementation() const
{
	return PathfindingComponent;
}

void ATBPlayerCharacter::OnMouseHover_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Character OnHover")));
}

void ATBPlayerCharacter::OnMouseUnHover_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Character OnUnHover")));
}

void ATBPlayerCharacter::OnMouseLeft_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Character OnMouseLeft")));
}

void ATBPlayerCharacter::OnMouseRight_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Character OnMouseRight")));
}

void ATBPlayerCharacter::OnMouseUnSelect_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Character OnUnSelect")));
}

