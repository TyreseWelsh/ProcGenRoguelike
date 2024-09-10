// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Node.generated.h"

class UPaperFlipbookComponent;

UCLASS()
class PROCMAPGENERATION_API ANode : public AActor
{
	GENERATED_BODY()

	// Components
	TObjectPtr<USceneComponent> Origin;
	TObjectPtr<UPaperFlipbookComponent> Sprite;
	
public:	
	// Sets default values for this actor's properties
	ANode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
