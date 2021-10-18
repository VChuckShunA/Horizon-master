// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Coins.generated.h"

/**
 * 
 */
class AHorizonCharacter;

UCLASS()
class HORIZON_API ACoins : public AItem
{
	GENERATED_BODY()

public:
	ACoins();

protected:
	UPROPERTY(EditAnywhere, Category = "Currency")
	int32 CoinsValue;

public:

	virtual void Use(AHorizonCharacter* Character) override;

	
};
