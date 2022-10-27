// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyClothSimulationGameMode.h"
#include "MyClothSimulationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyClothSimulationGameMode::AMyClothSimulationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
