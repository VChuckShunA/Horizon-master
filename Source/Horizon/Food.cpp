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


		UE_LOG(LogTemp, Warning, TEXT("Coins %d"), Character->Coins);

	}
}
