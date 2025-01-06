// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPoint.h"

#include "Components/SphereComponent.h"
#include "TileMapFunctionLibrary.h"
// Need to move pathfinding component to Plugin to get and cancel their move timer

// Sets default values
ATeleportPoint::ATeleportPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	RootComponent = Origin;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->InitSphereRadius(10.f);
	Collider->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Collider->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECR_Ignore);
	Collider->bHiddenInGame = false;
	Collider->SetupAttachment(Origin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetRelativeScale3D(FVector(0.2f, 0.2f, 0.2f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(Origin);
}

// Called when the game starts or when spawned
void ATeleportPoint::BeginPlay()
{
	Super::BeginPlay();

	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATeleportPoint::BeginOverlap);
}

// Called every frame
void ATeleportPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATeleportPoint::OnLeftClick_Implementation()
{
}

void ATeleportPoint::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActive)
	{
		//TeleportObject(OtherActor);
	}
}

void ATeleportPoint::TeleportObject(AActor* Object)
{
	if (bIsActive)
	{
		// If tile to teleport to is not blocked
		/*if (UTileMapFunctionLibrary::OccupyTile(Object))
		{*/
		// Get IHasPathfinding interface and call virtual EndMovement function
			TeleportLocation.Z = Object->GetActorLocation().Z;
			if(Object->SetActorLocation(TeleportLocation))
			{
				UTileMapFunctionLibrary::OccupyTile(Object);
			}
		//}
	}
}

