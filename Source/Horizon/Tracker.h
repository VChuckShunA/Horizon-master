// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.h"
#include "Tracker.generated.h"

UCLASS()
class HORIZON_API ATracker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		int MaxLaps;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		float GoldTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		float SilverTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		float BronzeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		float DefaultBestLap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		float DefaultBestTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		FString NameOfMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Settings")
		FString SaveGameName;

	//Checkpoints
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoints")
	TArray<ACheckpoint*> Checkpoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Checkpoints")
	int TotalCheckpoints;

	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void UpdateTimes();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void LapCheck(int Checkpoint);
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void ActivateCheckpoint(ACheckpoint* nextCheckpoint);\
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	bool RaceCompleteCheck();
	UFUNCTION(BlueprintCallable, Category = "Custom Events")
	void ApplySettings();

};
