// Fill out your copyright notice in the Description page of Project Settings.


#include "Clothing.h"

AClothing::AClothing()
{
	ClothingMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ClothingMesh"));
	SetRootComponent(ClothingMesh);

	ItemMesh->SetupAttachment(GetRootComponent());
}


void AClothing::Use(AHorizonCharacter* Character)
{
	if (Character)
	{

		UE_LOG(LogTemp, Warning, TEXT("USED THE CLOTHING!"));

	}
}