// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomExit.h"
#include "HasPathfinding.h"
#include "ExitGenerator.h"
#include "PathfindingComponent.h"

void ARoomExit::TeleportObject(AActor* Object)
{
	Super::TeleportObject(Object);

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
