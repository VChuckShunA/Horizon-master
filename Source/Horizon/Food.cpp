// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "HorizonCharacter.h"

AFood::AFood() :
HealthValue(10.f)
{


}

void AFood::Use(AHorizonCharacter* Character)
{
	if (Character)
	{

		if (ItemData.ItemState == EItemState::EIS_ForSale)
		{
			
		
		}
		else if (Character->Health < Character->MaxHealth)
		{
			Character->AddHealth(HealthValue);
			UE_LOG(LogTemp, Warning, TEXT("ItemPrice %d"), ItemData.ItemCost);
		}
		switch (Type)
		{
		case 1: Character->Food <= Character->MaxFood - 10.f ? Character->Food += 10.f : Character->Food = Character->MaxFood;
			break;
		case 2: Character->Water <= Character->MaxWater - 20.f ? Character->Water += 20.f : Character->Water = Character->MaxWater;
			break;
		case 3:
			Character->Food <= Character->MaxFood - 10.f ? Character->Food += 10.f : Character->Food = Character->MaxFood;
			Character->Water <= Character->MaxWater - 20.f ? Character->Water += 20.f : Character->Water = Character->MaxWater;
			break;
		default:
			break;
		}

		UE_LOG(LogTemp, Warning, TEXT("Coins %d"), Character->Coins);

	}
}
