// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionButton.h"

void UActionButton::NativeConstruct()
{
	Super::NativeConstruct();
}

void UActionButton::Init(AActor* NewOwner)
{
	Owner = NewOwner;
}

void UActionButton::OnButtonClicked(FString ClickedActionName)
{
}
