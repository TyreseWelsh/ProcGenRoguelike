// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HasPathfinding.h"
#include "Selectable.h"
#include "GameFramework/Actor.h"
#include "TBPlayerCharacter.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;
class UArrowComponent;
class UPlayerPathfindingComponent;
class UUnitInfoBar;

UCLASS()
class PROCGENTESTING_API ATBPlayerCharacter : public AActor, public IHasPathfinding, public ISelectable
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Origin;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* MainCollider;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TempMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* FrontFacingArrow;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UPlayerPathfindingComponent* PathfindingComponent;
	
public:	
	// Sets default values for this actor's properties
	ATBPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Init();
	
	UPathfindingComponent* GetPathfindingComponent() const;
	UPathfindingComponent* GetPathfindingComponent_Implementation() const;

	
	void OnMouseHover();
	void OnMouseHover_Implementation();
	void OnMouseUnHover();
	void OnMouseUnHover_Implementation();
	void OnMouseLeft();
	void OnMouseLeft_Implementation();
	void OnMouseRight();
	void OnMouseRight_Implementation();
	void OnMouseUnSelect();
	void OnMouseUnSelect_Implementation();

private:
	void DisplayInfo();
	
	bool bCanBeSelected;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "User Interface", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InfoWidgetClass;
	UUnitInfoBar* UnitInfoWidget;
	
};
