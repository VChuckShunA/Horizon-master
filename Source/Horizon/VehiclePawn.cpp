// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include"Camera/CameraComponent.h"
#include"Components/InputComponent.h"
#include"WheeledVehicleMovementComponent4W.h"

static const FName NAME_SteerInput("Steer");
static const FName NAME_ThrottleInput("Throttle");

AVehiclePawn::AVehiclePawn()
{
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());


	//Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 2.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 2.0f;


	//Torque setup
	Vehicle4W->MaxEngineRPM = 5700.f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f,400.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(1890.0f,500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(5730.0f,400.0f);


	//Adjust the steering
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(40.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.6f);


	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.65;

	//Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	//Create a sprint arm component to chase our camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 250.f;
	SpringArm->bUsePawnControlRotation = true;

	//Create the chase camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->FieldOfView = 90.f;


}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateInAirControls(DeltaTime);
}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(NAME_ThrottleInput, this, &AVehiclePawn::ApplyThrottle);
	PlayerInputComponent->BindAxis(NAME_SteerInput, this, &AVehiclePawn::ApplySteering);
	PlayerInputComponent->BindAxis("LookUp", this, &AVehiclePawn::Lookup);
	PlayerInputComponent->BindAxis("Turn", this, &AVehiclePawn::Turn);

	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AVehiclePawn::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AVehiclePawn::OnHandbrakeReleased);
}

void AVehiclePawn::ApplyThrottle(float Val)
{
	GetVehicleMovementComponent()->SetThrottleInput(Val);
}

void AVehiclePawn::ApplySteering(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}

void AVehiclePawn::Lookup(float Val)
{
	if (Val != 0.0f)
	{
		AddControllerPitchInput(Val);
	}
}

void AVehiclePawn::Turn(float Val)
{
	if (Val != 0.0f)
	{
		AddControllerYawInput(Val);
	}
}

void AVehiclePawn::OnHandbrakePressed()
{
	GetVehicleMovement()->SetHandbrakeInput(true);
}

void AVehiclePawn::OnHandbrakeReleased()
{
	GetVehicleMovement()->SetHandbrakeInput(false);
}

void AVehiclePawn::UpdateInAirControls(float DeltaTime)
{
	if (UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement()))
	{
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		const FVector TraceStart = GetActorLocation() + FVector(0.f, 0.f, 50.f);
		const FVector TraceEnd = GetActorLocation() - FVector(0.f,0.f,200.f);

		FHitResult Hit;

		//Check if the vehicle is flipped on its side and check if the vehicle is in air
		const bool bInAir = !GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
		const bool bNotGrounded = FVector::DotProduct(GetActorUpVector(), FVector::UpVector) < 0.1f;

		//Only allow in air-movement if we are not on the ground or are in the air
		if (bInAir || bNotGrounded)
		{
			const float ForwardInput = InputComponent->GetAxisValue(NAME_ThrottleInput);
			const float RightInput = InputComponent->GetAxisKeyValue(NAME_SteerInput);

			//If the vehicle is grounded, allow player to roll over
			const float AirMovementForcePitch = 3.f;
			const float AirMovementForceRoll = !bInAir && bNotGrounded ? 20.f : 3.f;
			if (UPrimitiveComponent* VehicleMesh = Vehicle4W->UpdatedPrimitive)
			{
				const FVector MovementVector = FVector(RightInput * -AirMovementForceRoll, ForwardInput * AirMovementForcePitch, 0.f) * DeltaTime * 200.f;
				const FVector NewAngularMovement = GetActorRotation().RotateVector(MovementVector);

				VehicleMesh->SetPhysicsAngularVelocity(NewAngularMovement, true);
			}
		}
	}
}
