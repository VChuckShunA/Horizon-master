// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/StaticMeshComponent.h"
#include "HorizonCharacter.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh");
	RootComponent = ItemMesh;

	bReplicates = true;

	ItemData.ItemClass = StaticClass();
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::Interact(class AHorizonCharacter* Character)
{
	if (HasAuthority() && Character)
	{
		Character->AddInventoryItem(ItemData);
		Destroy();
	}

}

void AItem::Use(AHorizonCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Using Item: %s"), *GetName());
}

