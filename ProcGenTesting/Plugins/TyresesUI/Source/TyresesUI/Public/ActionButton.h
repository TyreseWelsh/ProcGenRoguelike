// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TWUserWidget.h"
#include "ActionButton.generated.h"

class ATBActor;
class UTWButton;

/**
 * 
 */
UCLASS()
class TYRESESUI_API UActionButton : public UTWUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void Init(AActor* NewOwner);
	UFUNCTION(BlueprintCallable)
	void OnButtonClicked(FString ClickedActionName);

	UTWButton* GetActionButton() { return ActionButton; }
protected:
	UPROPERTY(meta=(BindWidget, AllowPrivateAccess))
	UTWButton* ActionButton;
	TObjectPtr<AActor> Owner;
};
