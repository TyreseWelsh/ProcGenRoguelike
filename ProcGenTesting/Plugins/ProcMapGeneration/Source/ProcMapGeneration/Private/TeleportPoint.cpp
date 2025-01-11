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
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, "TELEPORT YO AAA");

					TeleportLocation.Z = Object->GetActorLocation().Z;
					Object->SetActorLocation(TeleportLocation);
					GEngine->AddOnScreenDebugMessage(-1, 200.f, FColor::Green, FString::Printf(TEXT("%f %f %f"), TeleportLocation.X, TeleportLocation.Y, TeleportLocation.Z));

					// TODO: Current fix: need to instead call the current action's (move action) End function because currently it is not being called after teleporting
					//		 meaning that even though movement has stopped, we are not reverting back to an open action state
					//		 However, this is only a very specific fix and is not very generic if I want to use the teleport points for anything else (which is intended)
					//		 I probably need to separate this code into a derived exit teleporter class where this "TeleportObject" function is virtual so it can be overidden
					//		 Then this code will be for the player specifically
					//		 Will need to be extended with relevant room changing functionality such as moving all player units to this position and what not but its a start 
					ObjPathfindingComponent->EndMove();
				}
			}
		}
	}
}

