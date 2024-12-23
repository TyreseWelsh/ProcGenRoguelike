// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileColour.generated.h"

struct FLinearColor;
class AActor;

/**
 * 
 */
UCLASS()
class PROCMAPGENERATION_API UTileColour : public UObject
{
	GENERATED_BODY()

public:
	bool operator==(const UTileColour& other) const
	{
		return (this->Owner == other.Owner)
			&& (this->Colour == other.Colour);
	}

	void Init(FLinearColor NewColour, AActor* NewOwner)
	{
		Colour = NewColour;
		Owner = NewOwner;
	}
	
	FLinearColor GetColour() const { return Colour; }
	AActor* GetOwner() const { return Owner; }
private:
	FLinearColor Colour;
	UPROPERTY()
	AActor* Owner;
};
