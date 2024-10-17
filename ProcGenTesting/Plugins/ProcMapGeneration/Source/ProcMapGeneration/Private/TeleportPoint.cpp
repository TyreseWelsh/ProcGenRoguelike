// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPoint.h"

#include "IsTile.h"
#include "TileComponent.h"
#include "Components/SphereComponent.h"
#include "TileMapFunctionLibrary.h"

// Sets default values
ATeleportPoint::ATeleportPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("Origin"));
	RootComponent = Origin;

	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->InitSphereRadius(25.f);
	Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collider->SetCollisionObjectType(ECollisionChannel::ECC_WorldStatic);
	Collider->SetCollisionResponseToAllChannels(ECR_Overlap);
	Collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECR_Block);
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

	Collider->OnComponentHit.AddDynamic(this, &ATeleportPoint::OnHit);
	Collider->OnComponentBeginOverlap.AddDynamic(this, &ATeleportPoint::BeginOverlap);


}

// Called every frame
void ATeleportPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ATeleportPoint::OnLeftClick_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "TELEPORT");
}

void ATeleportPoint::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
}

void ATeleportPoint::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "overlap TELEPORT");

	if (bIsActive)
	{
		TeleportObject(OtherActor);
	}
}

void ATeleportPoint::TeleportObject(AActor* Object)
{
	if (bIsActive)
	{
		if (UTileMapFunctionLibrary::OccupyTile(Object))
		{
			Object->SetActorLocation(TeleportLocation);
		}
	}
}

