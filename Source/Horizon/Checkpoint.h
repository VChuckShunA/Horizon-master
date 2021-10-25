// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Checkpoint.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckpointCleared, int, NextCheckpoint);
UCLASS()
class HORIZON_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Triggers")
		USceneComponent* SceneComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Values")
	int CheckpointNumber;

	UPROPERTY(BlueprintCallable, Category = "EventDispatchers")
		FOnCheckpointCleared OnCheckpointCleared;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};