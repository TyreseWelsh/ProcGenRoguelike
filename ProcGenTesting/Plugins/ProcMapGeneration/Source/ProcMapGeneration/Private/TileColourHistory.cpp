// Fill out your copyright notice in the Description page of Project Settings.


#include "TileColourHistory.h"
#include "TileColour.h"

bool UTileColourHistory::Add(UTileColour* NewColour, FLinearColor& OutColour)
{
	if(IsValid(NewColour))
	{
		if(IsValid(NewColour->GetOwner()))
		{
			ColourHistory.Add(NewColour);
		}
	}

	return EvaluateColour(OutColour);
}

// Will return true if
bool UTileColourHistory::Remove(UTileColour* NewColour, FLinearColor& OutColour)
{
	if(IsValid(NewColour))
	{
		for(UTileColour* colour : ColourHistory)
		{
			if(colour->GetOwner() == NewColour->GetOwner()
				&& colour->GetColour() == NewColour->GetColour())
			{
				ColourHistory.Remove(colour);
			}
		}
	}
	
	return EvaluateColour(OutColour);
}

bool UTileColourHistory::EvaluateColour(FLinearColor& OutColour)
{
	if(ColourHistory.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("New Colour : %s"), *ColourHistory[ColourHistory.Num() - 1]->GetColour().ToString())
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("New Colour : %s"), *ColourHistory[ColourHistory.Num() - 1]->GetColour().ToString()));

		OutColour = ColourHistory[ColourHistory.Num() - 1]->GetColour();
		return true;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("No colours in history")));
	return false;
}
