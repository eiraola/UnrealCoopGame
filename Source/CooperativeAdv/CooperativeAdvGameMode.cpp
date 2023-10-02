// Copyright Epic Games, Inc. All Rights Reserved.

#include "CooperativeAdvGameMode.h"
#include "CooperativeAdvCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACooperativeAdvGameMode::ACooperativeAdvGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
