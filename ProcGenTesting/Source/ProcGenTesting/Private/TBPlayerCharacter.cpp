// Fill out your copyright notice in the Description page of Project Settings.


#include "TBPlayerCharacter.h"

#include "IsTile.h"
#include "PlayerPathfindingComponent.h"
#include "StrategyPlayerController.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "TileMapFunctionLibrary.h"
#include "UnitInfoBar.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATBPlayerCharacter::ATBPlayerCharacter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	RootComponent = Origin;
	
	MainCollider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MainCollider"));
	MainCollider->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	MainCollider->SetCollisionResponseToAllChannels(ECR_Overlap);
	MainCollider->SetRelativeLocation(FVector(0, 0, 45));
	MainCollider->bHiddenInGame = false;
	MainCollider->SetupAttachment(Origin);
	
	TempMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TempMesh"));
	TempMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TempMesh->SetupAttachment(MainCollider);

	FrontFacingArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("FrontFacingArrow"));
	FrontFacingArrow->SetupAttachment(TempMesh);
	FrontFacingArrow->SetHiddenInGame(false);

	PathfindingComponent = CreateDefaultSubobject<UPlayerPathfindingComponent>(TEXT("PathfindingComponent"));
}

// Called when the game starts or when spawned
void ATBPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	UTileMapFunctionLibrary::OccupyTile(this);
	Init();
}

// Called every frame
void ATBPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATBPlayerCharacter::Init()
{
	PathfindingComponent->Init();
}

UPathfindingComponent* ATBPlayerCharacter::GetPathfindingComponent_Implementation() const
{
	return PathfindingComponent;
}

void ATBPlayerCharacter::OnMouseHover_Implementation()
{
}

void ATBPlayerCharacter::OnMouseUnHover_Implementation()
{
}

void ATBPlayerCharacter::OnMouseLeft_Implementation()
{
	DisplayInfo();
}

void ATBPlayerCharacter::OnMouseRight_Implementation()
{
}

void ATBPlayerCharacter::OnMouseUnSelect_Implementation()
{
	PathfindingComponent->UnHighlightTiles(PathfindingComponent->GetTilesInRange(), FLinearColor::Blue);
}

void ATBPlayerCharacter::DisplayInfo()
{
	// Will open units UI at the bottom which displays their stats and any actions the player can take
	if(InfoWidgetClass)
	{
		UnitInfoWidget = CreateWidget<UUnitInfoBar>(GetWorld(), InfoWidgetClass);
		if(UnitInfoWidget)
		{
			UnitInfoWidget->Init(this);
			if(AStrategyPlayerController* StrategyController = Cast<AStrategyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
			{
				UnitInfoWidget->GetOnHoveredDelegate()->AddDynamic(StrategyController, &AStrategyPlayerController::DisableHovering);
				UnitInfoWidget->GetOnUnHoveredDelegate()->AddDynamic(StrategyController, &AStrategyPlayerController::EnableHovering);
			}
			UnitInfoWidget->AddToViewport();
		}
	}
}