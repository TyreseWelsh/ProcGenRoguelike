// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportPoint.h"

#include "ExitGenerator.h"
#include "HasPathfinding.h"
#include "PathfindingComponent.h"
#include "Components/SphereComponent.h"

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
	TeleportObject(OtherActor);
}

void ATeleportPoint::TeleportObject(AActor* Object)
{
	if (bIsActive)
	{
		if(Object->Implements<UHasPathfinding>())
		{
			if(IsValid(OwningGenerator))
			{
				OwningGenerator->ToggleExits(false);

				if(UPathfindingComponent* ObjPathfindingComponent = IHasPathfinding::Execute_GetPathfindingComponent(Object))
				{
					TeleportLocation.Z = Object->GetActorLocation().Z;
					Object->SetActorLocation(TeleportLocation);

					// TODO: Either move this functionality into a derived class (ARoomExit) or  have a boolean that determines if movement is stopped or not
					// Lore wise reason for movement stopping objects that go through is that the portals are unstable so they destabalize objects that go through
					ObjPathfindingComponent->EndMove();
				}
			}
		}
	}
}

