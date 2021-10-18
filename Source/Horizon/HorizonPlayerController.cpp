// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizonPlayerController.h"
#include "Blueprint/UserWidget.h"


AHorizonPlayerController::AHorizonPlayerController()
{
	
}



void AHorizonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Check our HUDOverlayClass TSubclassOF Variable
	if (IsLocalPlayerController())
	{
		if (HUDOverlayClass)
		{
			HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
			if (HUDOverlay)
			{
				HUDOverlay->AddToViewport();
				HUDOverlay->SetVisibility(ESlateVisibility::Visible);
			}
		}

		if (WInventory)
		{
			Inventory = CreateWidget<UUserWidget>(this, WInventory);
			if (Inventory)
			{
				Inventory->AddToViewport(1);
			
				Inventory->SetVisibility(ESlateVisibility::Hidden);
				
			}
		}
	}


}

void AHorizonPlayerController::Tick(float Deltatime)
{
	Super::Tick(Deltatime);
}

// Blueprint Callable Function , To Display Inventory (HUD button / KeyPress)
void AHorizonPlayerController::DisplayInventory_Implementation()
{
	if (Inventory)
	{
		bInventoryVisible = true;
		Inventory->SetVisibility(ESlateVisibility::Visible);
		FInputModeGameAndUI InputModeGameAndUI;
		SetInputMode(InputModeGameAndUI);
		bShowMouseCursor = true;
	}
}

void AHorizonPlayerController::RemoveInventory_Implementation()
{
	if (Inventory)
	{
		bInventoryVisible = false;
		Inventory->SetVisibility(ESlateVisibility::Hidden);
		FInputModeGameOnly InputModeGameOnly;
		SetInputMode(InputModeGameOnly);
		bShowMouseCursor = false;
	}
}



void AHorizonPlayerController::ToggleInventory()
{
	if (bInventoryVisible)
	{
		RemoveInventory();
	}
	else
	{
		DisplayInventory();
	}
}

void AHorizonPlayerController::ShowInventory(bool Visible)
{
	if (Visible)
	{
		DisplayInventory();
	}
	else
	{
		RemoveInventory();
	}
}


