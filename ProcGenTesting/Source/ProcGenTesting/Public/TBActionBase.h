// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBActionBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnActionEndSignature);

class AActor;
class UTileComponent;
class UTWUserWidget;

USTRUCT(BlueprintType)
struct FActionData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UTWUserWidget> ButtonClass;
};

/**
 * 
 */
UCLASS(Blueprintable)
class PROCGENTESTING_API UTBActionBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(AActor* NewUnit);
	UFUNCTION()
	virtual void EndAction();
	virtual void OnHover(UTileComponent* CurrentHoveredTile, UTileComponent* NewHoveredTile);
	virtual void OnLeftClick(UTileComponent* SelectedTile);
	virtual void OnRightClick();

	virtual void DisableHover(UTileComponent* CurrentHoveredTile);
	virtual void EnableHover();

	FActionData GetData() { return ActionData; }
	
	FOnActionEndSignature* GetActionEndDelegate() { return &ActionEndDelegate; }
	
	
protected:
	bool bCanHover = true;
	UPROPERTY(EditAnywhere)
	FActionData ActionData;
	
	FOnActionEndSignature ActionEndDelegate;
};
