// Copyright Epic Games, Inc. All Rights Reserved.

#include "HorizonGameMode.h"
#include "HorizonCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHorizonGameMode::AHorizonGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
