// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameMode.h"

AHSGameMode::AHSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> CharacterPlayerClassRef = TEXT("/Script/HackAndSlash.HSCharacterPlayer");
	if (CharacterPlayerClassRef.Class)
	{
		DefaultPawnClass = CharacterPlayerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef = TEXT("/Script/HackAndSlash.HSPlayerController");
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
}
