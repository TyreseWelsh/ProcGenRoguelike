// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TWUserWidget.h"
#include "UnitInfoBar.generated.h"

/**
 * 
 */
UCLASS()
class TYRESESUI_API UUnitInfoBar : public UTWUserWidget
{
	GENERATED_BODY()

public:
	void Init(AActor* NewOwner);
	void MoveAction();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetOwner() { return Owner; }
private:
	UPROPERTY()
	TObjectPtr<AActor> Owner;
};
