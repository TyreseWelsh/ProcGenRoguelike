// Fill out your copyright notice in the Description page of Project Settings.


#include "TileNode.h"

#include "PaperFlipbookComponent.h"
#include "TileComponent.h"

// Sets default values
ATileNode::ATileNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//
	TileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = TileRoot;

	Sprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	Sprite->SetRelativeLocation(FVector(16.f, 16.f, 0.f));
	Sprite->SetRelativeRotation(FRotator(0.f, 0.f, -90.f));
	Sprite->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Sprite->SetupAttachment(RootComponent);

	TileComponent = CreateDefaultSubobject<UTileComponent>(TEXT("TileComponent"));
	TileComponent->SetRelativeLocation(FVector(16.f, 16.f, 16.f));
	TileComponent->InitBoxExtent(FVector(16.f, 16.f, 16.f));
	TileComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TileComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	TileComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATileNode::BeginPlay()
{
	Super::BeginPlay();

	DynamicMatInstance = UMaterialInstanceDynamic::Create(Sprite->GetMaterial(0), Sprite);
	Sprite->SetMaterial(0, DynamicMatInstance);

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

void ATileNode::InitWall()
{
	// No bindings since walls are not interactable
	Sprite->SetFlipbook(WallFlipbook);
}

void ATileNode::InitPath()
{
	Sprite->SetFlipbook(PathFlipbooks[HeightLevel]);
	
	TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::BindHover);
	TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::BindUnHover);
	TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::BindLeftClick);
	TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::BindUnSelect);
}

void ATileNode::InitGround()
{
	Sprite->SetFlipbook(GroundFlipbooks[HeightLevel]);
	
	TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::BindHover);
	TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::BindUnHover);
	TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::BindLeftClick);
	TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::BindUnSelect);
}

void ATileNode::InitExit()
{
	Sprite->SetFlipbook(ExitFlipbooks[HeightLevel]);

	TileComponent->GetMouseHoverDelegate()->AddUObject(this, &ATileNode::BindHover);
	TileComponent->GetMouseUnHoverDelegate()->AddUObject(this, &ATileNode::BindUnHover);
	TileComponent->GetLeftClickDelegate()->AddUObject(this, &ATileNode::BindLeftClick);
	TileComponent->GetUnSelectDelegate()->AddUObject(this, &ATileNode::BindUnSelect);
}

void ATileNode::BindHover()
{
	FLinearColor NewColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), NewColour))
	{
		NewColour += HoverColour;
		NewColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), NewColour);
	}
}

void ATileNode::BindUnHover()
{
	FLinearColor NewColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), NewColour))
	{
		NewColour -= HoverColour;
		NewColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), NewColour);
	}
}

void ATileNode::BindLeftClick()
{
	FLinearColor NewColour;
	if(DynamicMatInstance->GetVectorParameterValue(FName("OverlayColour"), NewColour))
	{
		NewColour += SelectColour;
		NewColour.A = 1;
		DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), NewColour);
	}
}

void ATileNode::BindUnSelect()
{
	DynamicMatInstance->SetVectorParameterValue(FName("OverlayColour"), FLinearColor::White);
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

