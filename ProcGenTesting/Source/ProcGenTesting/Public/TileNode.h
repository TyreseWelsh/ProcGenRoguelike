// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IsTile.h"
#include "Selectable.h"
#include "TileNode.generated.h"

class UTileComponent;
class UTileColourHistory;
class UTileColour;

UCLASS()
class PROCGENTESTING_API ATileNode : public AActor, public IIsTile, public ISelectable
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
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UTileComponent* GetTileComponent();
	virtual UTileComponent* GetTileComponent_Implementation() override { return TileComponent; }

	void AddTileColour(UTileColour* NewColour);
	void AddTileColour_Implementation(UTileColour* NewTileColour);
	
	void SubtractTileColour(UTileColour* NewColour);
	void SubtractTileColour_Implementation(UTileColour* NewTileColour);
	
	void EnableHighlight();
	void DisableHighlight();

	void BindSelectDelegates(bool BindHover, bool BindUnHover, bool BindLeftMouse, bool BindRightMouse, bool BindUnSelect);
	
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

	UPROPERTY()
	UTileColourHistory* ColourHistory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor HoverColour;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FLinearColor SelectColour;
};
