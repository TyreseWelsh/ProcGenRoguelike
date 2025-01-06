// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "TeleportPoint.generated.h"

class USphereComponent;

UCLASS()
class PROCMAPGENERATION_API ATeleportPoint : public AActor, public IInteractable
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USceneComponent* Origin;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	USphereComponent* Collider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
public:	
	// Sets default values for this actor's properties
	ATeleportPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnLeftClick_Implementation() override;
	
	USphereComponent* GetCollider() { return Collider; }
	UStaticMeshComponent* GetMesh() { return Mesh; }

	UFUNCTION(BlueprintCallable)
	void TeleportObject(AActor* Object);
	
	UFUNCTION(BlueprintCallable)
	bool GetIsActive() const { return bIsActive; }
	UFUNCTION(BlueprintCallable)
	void SetIsActive(bool NewActive) { bIsActive = NewActive; }

	void SetTeleportLocation(FVector NewLocation) { TeleportLocation = NewLocation; }
	
	
private:
	//
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//
	bool bIsActive = true;
	FVector TeleportLocation = FVector::ZeroVector; 
};
