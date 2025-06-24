// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameMode.h"

AHSGameMode::AHSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> CharacterPlayerClassRef = TEXT("/Script/Engine.Blueprint'/Game/HackAndSlash/Blueprint/BP_HSCharacterPlayer.BP_HSCharacterPlayer_C'");
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
