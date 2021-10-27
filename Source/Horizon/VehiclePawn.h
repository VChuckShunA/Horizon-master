// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "VehiclePawn.generated.h"

/**
 * 
 */
UCLASS()
class HORIZON_API AVehiclePawn : public AWheeledVehicle
{
	GENERATED_BODY()

public: 
	AVehiclePawn();
	AVehiclePawn * VehiclePawn;
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)override;

	//Throttle/Steering
	void ApplyThrottle(float Val);
	void ApplySteering(float Val);

	//Look around
	void Lookup(float Val);
	void Turn(float Val);

	//Handbreak
	void OnHandbrakePressed();
	void OnHandbrakeReleased();

	//Update in air physics
	void UpdateInAirControls(float DeltaTime);


protected: 
	//Spring arm that will offset the camera
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	//Camera component that will be our viewpor
	UPROPERTY(Category = Camera, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;


	
};
