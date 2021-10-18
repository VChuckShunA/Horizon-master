// Fill out your copyright notice in the Description page of Project Settings.


#include "Coins.h"
#include "HorizonCharacter.h"

ACoins::ACoins()
{
	CoinsValue = 3;

	ItemData.ItemClass = StaticClass();
}

void ACoins::Use(AHorizonCharacter* Character)
{
	if (Character)
	{
		Character->AddCoins(CoinsValue);
	}

}
