// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnrealBrawlerGameMode.h"
#include "UnrealBrawlerCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealBrawlerGameMode::AUnrealBrawlerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
