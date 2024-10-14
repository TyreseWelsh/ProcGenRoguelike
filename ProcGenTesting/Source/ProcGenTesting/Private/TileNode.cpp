// Fill out your copyright notice in the Description page of Project Settings.


#include "TileNode.h"

#include "Interactable.h"
#include "TileComponent.h"

// Sets default values
ATileNode::ATileNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	TileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = TileRoot;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//Mesh->SetRelativeLocation(FVector(Mesh->))
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
}

// Called every frame
void ATileNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATileNode::AddToOverlayColour(FLinearColor NewColour)
{
	FLinearColor OverlayColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), OverlayColour))
	{
		OverlayColour += NewColour;
		OverlayColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), OverlayColour);
	}
}

void ATileNode::RemoveFromOverlayColour(FLinearColor NewColour)
{
	FLinearColor OverlayColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), OverlayColour))
	{
		OverlayColour -= NewColour;
		OverlayColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), OverlayColour);
	}
}

void ATileNode::EnableHighlight()
{
	float HighlightLevel = 0.5;
	DynamicMatInstance->SetScalarParameterValue(FName("HighlightLevel"), HighlightLevel);
}

void ATileNode::DisableHighlight()
{
	float HighlightLevel = 0;
	DynamicMatInstance->SetScalarParameterValue(FName("HighlightLevel"), HighlightLevel);
}

void ATileNode::InitWall()
{
	// No bindings since walls are not interactable
	//Sprite->SetFlipbook(WallFlipbook);
}

void ATileNode::InitPath()
{
	//Sprite->SetFlipbook(PathFlipbooks[HeightLevel]);

	FLinearColor BaseColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("BaseColour"), BaseColour))
	{
		BaseColour = FLinearColor(0.2, 0.1, 0.03, 1);
		BaseColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("BaseColour"), BaseColour);
	}

	TileComponent->GetMouseHoverDelegate()->RemoveAll(this);
	TileComponent->GetMouseUnHoverDelegate()->RemoveAll(this);
	TileComponent->GetLeftClickDelegate()->RemoveAll(this);
	TileComponent->GetUnSelectDelegate()->RemoveAll(this);
	
	TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::BindHover);
	TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::BindUnHover);
	TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::BindLeftClick);
	TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::BindUnSelect);
}

void ATileNode::InitGround()
{
	//Sprite->SetFlipbook(GroundFlipbooks[HeightLevel]);
	
	TileComponent->GetMouseHoverDelegate()->RemoveAll(this);
	TileComponent->GetMouseUnHoverDelegate()->RemoveAll(this);
	TileComponent->GetLeftClickDelegate()->RemoveAll(this);
	TileComponent->GetUnSelectDelegate()->RemoveAll(this);
	
	TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::BindHover);
	TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::BindUnHover);
	TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::BindLeftClick);
	TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::BindUnSelect);
}

void ATileNode::InitExit()
{
	//Sprite->SetFlipbook(ExitFlipbooks[HeightLevel]);
	FLinearColor BaseColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("BaseColour"), BaseColour))
	{
		BaseColour = FLinearColor::Red;
		BaseColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("BaseColour"), BaseColour);
	}

	TileComponent->GetMouseHoverDelegate()->RemoveAll(this);
	TileComponent->GetMouseUnHoverDelegate()->RemoveAll(this);
	TileComponent->GetLeftClickDelegate()->RemoveAll(this);
	TileComponent->GetUnSelectDelegate()->RemoveAll(this);
	
	TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::BindHover);
	TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::BindUnHover);
	TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::BindLeftClick);
	TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::BindUnSelect);
}

void ATileNode::BindHover()
{
	AddToOverlayColour(HoverColour);

	EnableHighlight();
}

void ATileNode::BindUnHover()
{
	RemoveFromOverlayColour(HoverColour);
	if(!bIsSelected)
	{
		DisableHighlight();
	}
}

void ATileNode::BindLeftClick()
{
	bIsSelected = true;
	AddToOverlayColour(SelectColour);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("x=%i , y=%i"), TileComponent->GetRoomIndexX(), TileComponent->GetRoomIndexY()));
	if (AActor* OccupyingObject = TileComponent->GetOccupyingObject())
	{
		if (OccupyingObject->Implements<UInteractable>())
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("INTERACTING")));

			IInteractable::Execute_OnLeftClick(OccupyingObject);
		}
	}
}

void ATileNode::BindUnSelect()
{
	bIsSelected = false;
	RemoveFromOverlayColour(SelectColour);
	DisableHighlight();
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

