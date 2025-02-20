// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionButton.h"9
#include "TWButton.h"
#include "TBActionBase.h"
#include "TWTextBlock.h"

void UActionButton::NativeConstruct()
{
	Super::NativeConstruct();

	ActionButton->OnClicked.AddDynamic(this, &UActionButton::OnButtonPressed);
}

void UActionButton::Init(AActor* NewOwner, FActionData NewActionData)
{
	Owner = NewOwner;

	ActionName->SetText(FText::FromString(NewActionData.Name));
	ActionCost->SetText(FText::FromString(FString::Printf(NewActionData.Cost)));
}

void UActionButton::OnButtonPressed()
{
}

void UActionButton::EnableButton()
{
	bEnabled = true;
}

void UActionButton::DisableButton()
{
	bEnabled = false;
}
