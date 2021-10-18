// Fill out your copyright notice in the Description page of Project Settings.


#include "HorizonAnimInstance.h"
#include "HorizonCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

UHorizonAnimInstance::UHorizonAnimInstance() :
Speed(0.f),
bIsInAir(false),
bIsAccelerating(false)

{
}

void UHorizonAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (HorizonCharacter == nullptr)
	{
		HorizonCharacter = Cast<AHorizonCharacter>(TryGetPawnOwner());
	}
	if (HorizonCharacter)
	{
		FVector Velocity{ HorizonCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = HorizonCharacter->GetCharacterMovement()->IsFalling();

		if (HorizonCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
		if (HorizonCharacter->GetIsInWater())
		{
			float CharacterZ = HorizonCharacter->GetActorLocation().Z; //To compare the initial Z after overlapping, to detect water height

			if (HorizonCharacter->InitialWaterHeight - CharacterZ > 96.f)
			{
				bIsSwimming = true; //Handles Start swimming animation
				// Character->Start Swimming should be called once
				HorizonCharacter->StartSwimming();
			}
			else
			{
				bIsSwimming = false; //Handles Stop swimming animation
				// Character->Stop Swimming should be called once
				HorizonCharacter->StopSwimming();
			}
		}
	}

}

void UHorizonAnimInstance::NativeInitializeAnimation()
{
	HorizonCharacter = Cast<AHorizonCharacter>(TryGetPawnOwner());
}
