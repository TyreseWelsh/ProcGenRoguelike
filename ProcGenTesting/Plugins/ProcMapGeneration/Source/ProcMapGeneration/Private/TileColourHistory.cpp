// Fill out your copyright notice in the Description page of Project Settings.


#include "TileColourHistory.h"
#include "TileColour.h"

FLinearColor UTileColourHistory::Add(UTileColour* NewColour)
{
	if(NewColour != nullptr && IsValid(NewColour))
	{
		if(NewColour->GetOwner() != nullptr && IsValid(NewColour->GetOwner()))
		{
			ColourHistory.Add(NewColour);
		}
		else
		{
			UE_LOG(LogTemp, Display, TEXT("Colour owner not valid"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("ERROR: Invalid Tile Colour To Add...")));
	}
	return EvaluateColour();
}

FLinearColor UTileColourHistory::Remove(UTileColour* NewColour)
{
	if(IsValid(NewColour))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Num colours before = %i"), ColourHistory.Num()));

		for(UTileColour* colour : ColourHistory)
		{
			if(colour->GetOwner() == NewColour->GetOwner()
				&& colour->GetColour() == NewColour->GetColour())
			{
				ColourHistory.Remove(colour);
			}
		}

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Num colours after = %i"), ColourHistory.Num()));
	}

	return EvaluateColour();
}

FLinearColor UTileColourHistory::EvaluateColour()
{
	if(ColourHistory.Num() > 0)
	{
		//UE_LOG(LogTemp, Warning, TEXT("New Colour : %s"), *ColourHistory[ColourHistory.Num() - 1]->GetColour().ToString())
		//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("New Colour : %s"), *ColourHistory[ColourHistory.Num() - 1]->GetColour().ToString()));

		return ColourHistory[ColourHistory.Num() - 1]->GetColour();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, FString::Printf(TEXT("No colours in history")));
	return FLinearColor::White;
}
