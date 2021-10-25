// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RaceHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class HORIZON_API URaceHUDWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* TextLabel;
	
};
