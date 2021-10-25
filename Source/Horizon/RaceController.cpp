// Fill out your copyright notice in the Description page of Project Settings.


#include "RaceController.h"
#include "Blueprint/userWidget.h"
#include "RaceHUDWidget.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "VehiclePawn.h"
#include "Templates/Casts.h"
void ARaceController::initText()
{
}

ARaceController::ARaceController()
{

}

void ARaceController::BeginPlay()
{
	Super::BeginPlay();

	//Create Widget (Hud) and get a reference
	if (IsValid(WidgetClass))
	{
		HUDReference = Cast<URaceHUDWidget>(CreateWidget(GetWorld(),WidgetClass));
		//Add HUD to viewport
		if (HUDReference != nullptr)
		{
			HUDReference->AddToViewport();
		}
	}

	

	//Get Player Pawn
	//APawn* VehiclePawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//VehiclePawn = Cast<AVehiclePawn>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));


}

void ARaceController::StartGameSetup()
{
}

void ARaceController::Restart()
{
}

void ARaceController::UpdateLap()
{
}

void ARaceController::RespawnVehicle()
{
}

void ARaceController::StartRaceTime()
{
}

void ARaceController::StopRaceTime()
{
}

void ARaceController::StartLapTime()
{
}

void ARaceController::StopLapTime()
{
}
