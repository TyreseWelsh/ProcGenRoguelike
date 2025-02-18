// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TBPlayer.generated.h"

class USceneComponent;
class UCameraComponent;
class ATBActor;

UCLASS()
class PROCMAPGENERATION_API ATBPlayer : public APawn
{
	GENERATED_BODY()

	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* Origin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
public:
	// Sets default values for this pawn's properties
	ATBPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UCameraComponent* GetCameraComponent() { return Camera; }

private:
	UPROPERTY()
	TArray<ATBActor*> PlayerUnits;
};
