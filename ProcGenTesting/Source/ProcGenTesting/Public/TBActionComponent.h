// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TBActionComponent.generated.h"

class UTBActionBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROCGENTESTING_API UTBActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTBActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCurrentAction(UTBActionBase* NewAction) { CurrentAction = NewAction; }
	UTBActionBase* GetCurrentAction() { return CurrentAction; }

	
	UPROPERTY(EditAnywhere)
	TArray<UTBActionBase*> AvailableActions;
private:
	TObjectPtr<UTBActionBase> CurrentAction;

};
