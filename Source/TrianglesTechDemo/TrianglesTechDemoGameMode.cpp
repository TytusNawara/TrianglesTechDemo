// Copyright Epic Games, Inc. All Rights Reserved.

#include "TrianglesTechDemoGameMode.h"
#include "TrianglesTechDemoCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATrianglesTechDemoGameMode::ATrianglesTechDemoGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
