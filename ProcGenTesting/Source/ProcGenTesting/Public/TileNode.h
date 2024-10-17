// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IsTile.h"
#include "TileNode.generated.h"

class UTileComponent;

UCLASS()
class PROCGENTESTING_API ATileNode : public AActor, public IIsTile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* TileRoot;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UTileComponent* TileComponent;
	
public:	
	// Sets default values for this actor's properties
	ATileNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BindHover();
	virtual void BindUnHover();
	virtual void BindLeftClick();
	virtual void BindUnSelect();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UTileComponent* GetTileComponent();
	virtual UTileComponent* GetTileComponent_Implementation() override { return TileComponent; }

	void AddTileColour(FLinearColor NewColour);
	void AddTileColour_Implementation(FLinearColor NewColour);
	
	void SubtractTileColour(FLinearColor NewColour);
	void SubtractTileColour_Implementation(FLinearColor NewColour);
	
	void EnableHighlight();
	void DisableHighlight();
	
private:
	void InitWall();
	void InitPath();
	void InitGround();
	void InitExit();



	void CalculateHeightLevel();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UMaterialInstanceDynamic* DynamicMatInstance;
	
	TArray<float> HeightThresholds;
	int HeightLevel;
	UPROPERTY()
	int HighlightCounter = 0;
	bool bIsSelected = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor HoverColour;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor SelectColour;
};
