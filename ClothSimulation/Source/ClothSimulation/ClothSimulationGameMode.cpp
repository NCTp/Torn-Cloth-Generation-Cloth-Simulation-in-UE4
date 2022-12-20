// Copyright Epic Games, Inc. All Rights Reserved.

#include "ClothSimulationGameMode.h"
#include "ClothSimulationCharacter.h"
#include "UObject/ConstructorHelpers.h"

AClothSimulationGameMode::AClothSimulationGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
