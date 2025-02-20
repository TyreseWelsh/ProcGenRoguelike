// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitInfoBar.h"

//#include "../../../../ProcMapGeneration/Source/ProcMapGeneration/Public/TBActor.h"
//#include "../../ProcMapGeneration/Source/ProcMapGeneration/Public/TBActor.h"
#include <TBActor.h>

#include "ActionButton.h"
#include "TBActionBase.h"
#include "TWButton.h"
#include "Components/HorizontalBox.h"

void UUnitInfoBar::Init(AActor* NewOwner, TArray<UTBActionBase*> Actions)
{
	Owner = Cast<ATBActor>(NewOwner);

	for(UTBActionBase* TBAction : Actions)
	{
		if(UActionButton* NewButton = CreateWidget<UActionButton>(GetWorld(), TBAction->GetData().ButtonClass))
		{
			NewButton->GetActionButton()->OnHovered.AddDynamic(this, &UUnitInfoBar::BroadcastOnHovered);
			NewButton->GetActionButton()->OnUnhovered.AddDynamic(this, &UUnitInfoBar::BroadcastOnUnHovered);
			NewButton->Init(Owner);
			actionBox->AddChildToHorizontalBox(NewButton);
		}
	}
}

void UUnitInfoBar::BroadcastOnHovered()
{
	if(OnHoveredDelegate.IsBound())
	{
		OnHoveredDelegate.Broadcast();
	}
}

void UUnitInfoBar::BroadcastOnUnHovered()
{
	if(OnUnHoveredDelegate.IsBound())
	{
		OnUnHoveredDelegate.Broadcast();
	}
}

void UUnitInfoBar::EndTurn()
{
	if(Owner)
	{
		Owner->EndTurn();
	}
}
