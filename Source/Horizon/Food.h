// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Food.generated.h"

/**
 * 
 */
class AHorizonCharacter;

UCLASS()
class HORIZON_API AFood : public AItem
{
	GENERATED_BODY()

public:
	AFood();

protected:
	UPROPERTY(EditAnywhere, Category = "Food")
	float HealthValue;


public:

	virtual void Use(AHorizonCharacter* Character) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Type;

};
