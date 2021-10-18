// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HorizonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HORIZON_API AHorizonPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHorizonPlayerController();

	/* Reference to the Overall HUD Overlay BluePrint Class*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget>HUDOverlayClass;

	/* Variable that hold the HUD Overlay Widget after creating it*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widgets", meta = (AllowPrivateAccess = "true"))
		UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WInventory;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* Inventory;



	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool bInventoryVisible;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
	bool bVendorInventoryVisible;


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void DisplayInventory();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "HUD")
		void RemoveInventory();

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool Visible);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float Deltatime) override;

private:

};
