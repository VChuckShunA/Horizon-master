// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RaceController.generated.h"

/**
 * 
 */

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(RaceHastStarted,bool)
UCLASS()
class HORIZON_API ARaceController : public APlayerController
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = "Widget")
		TSubclassOf<UUserWidget> WidgetClass;
	UPROPERTY(VisibleInstanceOnly, Category = "Widget")
		class URaceHUDWidget* HUDReference;
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	int actualLap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	int maxLaps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float actualRaceTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float actualLapTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float goldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float silverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float bronzeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float bestLapTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float bestRaceTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float defaultBestLapTime; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	float defaultBestRaceTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	bool raceComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	bool raceStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	FTransform respawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Value Variables")
	FString saveSlot;

	//Text Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText raceTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText currentLapText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText mapLapText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText goldTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText silverTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText bronzeTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText bestTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText bestLapText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText nameOnMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text Variables")
	FText readyText;


	//Custom Events
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void StartGameSetup();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void Restart();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void UpdateLap();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void RespawnVehicle();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void StartRaceTime();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void StopRaceTime();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void StartLapTime();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
		void StopLapTime();

	void initText();
	void saveGameCheck();
	void saveTheGame();
	void loadTheGame();
	void lapTimeCheck();
	void raceTimeCheck();
	void updateGoals();
	ARaceController();
};
