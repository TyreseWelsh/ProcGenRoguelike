// Fill out your copyright notice in the Description page of Project Settings.


#include "TileNode.h"

#include "Interactable.h"
#include "TileComponent.h"
#include "TileColourHistory.h"
#include "TileColour.h"

#include "Selectable.h"

// Sets default values
ATileNode::ATileNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	TileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = TileRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetupAttachment(RootComponent);

	TileComponent = CreateDefaultSubobject<UTileComponent>(TEXT("TileComponent"));
	TileComponent->SetRelativeLocation(FVector(0.f, 0.f, 16.f));
	TileComponent->InitBoxExtent(FVector(16.f, 16.f, 16.f));
	TileComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TileComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	TileComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATileNode::BeginPlay()
{
	Super::BeginPlay();

	DynamicMatInstance = UMaterialInstanceDynamic::Create(Mesh->GetMaterial(0), Mesh);
	Mesh->SetMaterial(0, DynamicMatInstance);

	TileComponent->GetTileTypeToWallDelegate()->AddUObject(this, &ATileNode::InitWall);
	TileComponent->GetTileTypeToPathDelegate()->AddUObject(this, &ATileNode::InitPath);
	TileComponent->GetTileTypeToGroundDelegate()->AddUObject(this, &ATileNode::InitGround);
	TileComponent->GetTileTypeToExitDelegate()->AddUObject(this, &ATileNode::InitExit);

	CalculateHeightLevel();

	ColourHistory = NewObject<UTileColourHistory>(); 
}

// Called every frame
void ATileNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileNode::AddTileColour_Implementation(UTileColour* NewTileColour)
{
	/*FLinearColor OverlayColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), OverlayColour))
	{
		OverlayColour = ColourHistory->Add(NewTileColour);
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), OverlayColour);
	}
	EnableHighlight();*/

	FLinearColor OverlayColour;
	if(ColourHistory->Add(NewTileColour, OverlayColour))
	{
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), OverlayColour);
		EnableHighlight();
	}
}

void ATileNode::SubtractTileColour_Implementation(UTileColour* NewTileColour)
{
	/*FLinearColor OverlayColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), OverlayColour))
	{
		OverlayColour = ColourHistory->Remove(NewTileColour);
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), OverlayColour);
	}
	DisableHighlight();*/

	FLinearColor OverlayColour;
	if(ColourHistory->Remove(NewTileColour, OverlayColour))
	{
		HighlightCounter--;
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), OverlayColour);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Red, FString::Printf(TEXT("Unihighlight Tile")));

		DisableHighlight();
	}
	GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Purple, FString::Printf(TEXT("Finished subtracting color")));

}

// ERROR: PROBLEM OF SOME TILES NOT HIGHLIGHTING
void ATileNode::EnableHighlight()
{
	HighlightCounter++;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("%s is highlighted - %i"), *GetName(), HighlightCounter));

	/*if (HighlightCounter > 0)
	{*/
		float HighlightLevel = 0.5;
		DynamicMatInstance->SetScalarParameterValue(FName("HighlightLevel"), HighlightLevel);
	//}
}

void ATileNode::DisableHighlight()
{
	HighlightCounter--;
	//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("%s's HighlightCounter = %i"), *GetName(), HighlightCounter));

	if (HighlightCounter <= 0)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Yellow, FString::Printf(TEXT("%s is unhighlighted - %i"), *GetName(), HighlightCounter));

		HighlightCounter = 0;
		float HighlightLevel = 0;
		DynamicMatInstance->SetScalarParameterValue(FName("HighlightLevel"), HighlightLevel);
	}
}

void ATileNode::BindSelectDelegates(bool BindHover, bool BindUnHover, bool BindLeftMouse, bool BindRightMouse, bool BindUnSelect)
{
	TileComponent->GetMouseHoverDelegate()->RemoveAll(this);
	TileComponent->GetMouseUnHoverDelegate()->RemoveAll(this);
	TileComponent->GetLeftClickDelegate()->RemoveAll(this);
	TileComponent->GetUnSelectDelegate()->RemoveAll(this);

	if(BindHover)
	{
		TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::OnMouseHover_Implementation);
	}
	if(BindUnHover)
	{
		TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::OnMouseUnHover_Implementation);
	}
	if(BindLeftMouse)
	{
		TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::OnMouseLeft_Implementation);
	}
	if(BindRightMouse)
	{
	}
	if(BindUnSelect)
	{
		TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::OnMouseUnSelect_Implementation);
	}
}

void ATileNode::InitWall()
{
	// No bindings since walls are not interactable
}

void ATileNode::InitPath()
{
	FLinearColor BaseColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("BaseColour"), BaseColour))
	{
		BaseColour = FLinearColor(0.2, 0.1, 0.03, 1);
		BaseColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("BaseColour"), BaseColour);
	}

	BindSelectDelegates(true, true, true, true, true);
}

void ATileNode::InitGround()
{
	BindSelectDelegates(true, true, true, true, true);
}

void ATileNode::InitExit()
{
	FLinearColor BaseColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("BaseColour"), BaseColour))
	{
		BaseColour = FLinearColor::Red;
		BaseColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("BaseColour"), BaseColour);
	}

	BindSelectDelegates(true, true, true, true, true);
}

void ATileNode::OnMouseHover_Implementation()
{
	TObjectPtr<UTileColour> NewTileColour = NewObject<UTileColour>();
	NewTileColour->Init(HoverColour, this);
	AddTileColour_Implementation(NewTileColour);

}

void ATileNode::OnMouseUnHover_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Black, FString::Printf(TEXT("UNHOVER")));

	TObjectPtr<UTileColour> NewTileColour = NewObject<UTileColour>();
	NewTileColour->Init(HoverColour, this);
	SubtractTileColour_Implementation(NewTileColour);
}

void ATileNode::OnMouseLeft_Implementation()
{
	bIsSelected = true;
	
	UTileColour* NewTileColour = NewObject<UTileColour>();
	NewTileColour->Init(SelectColour, this);
	AddTileColour_Implementation(NewTileColour);


	//
	if (AActor* OccupyingObject = TileComponent->GetOccupyingObject())
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Tile selected...")));

		ISelectable::Execute_OnMouseLeft(OccupyingObject);
		//IInteractable::Execute_OnLeftClick(OccupyingObject);
	}
}

void ATileNode::OnMouseRight_Implementation()
{
}

void ATileNode::OnMouseUnSelect_Implementation()
{
	bIsSelected = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White, FString::Printf(TEXT("Tile UNSELECTED...")));

	UTileColour* NewTileColour = NewObject<UTileColour>();
	NewTileColour->Init(SelectColour, this);
	SubtractTileColour_Implementation(NewTileColour);
}

void ATileNode::CalculateHeightLevel()
{
	for(int i = 0; i < HeightThresholds.Num(); i++)
	{
		if(TileComponent->GetTileHeight() < HeightThresholds[i])
		{
			HeightLevel = HeightThresholds[i] - 1;
			break;
		}
	}
}

