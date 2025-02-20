// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TWUserWidget.h"
#include "ActionButton.generated.h"

class ATBActor;
class UTWButton;
class UTWTextBlock;
struct FActionData;

/**
 * 
 */
UCLASS()
class PROCGENTESTING_API UActionButton : public UTWUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Init(AActor* NewOwner, FActionData NewActionData);
	UFUNCTION(BlueprintCallable)
	virtual void OnButtonPressed();
	UFUNCTION(BlueprintCallable)
	void EnableButton();
	UFUNCTION(BlueprintCallable)
	void DisableButton();
	
	UTWButton* GetActionButton() { return ActionButton; }
protected:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	UTWButton* ActionButton;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	UTWTextBlock* ActionName;
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	UTWTextBlock* ActionCost;

	TObjectPtr<AActor> Owner;
	bool bEnabled = true;
};
