// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/HSGameMode.h"
#include "Player/HSPlayerController.h"


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

	ClearScore = 3;
	CurrentScore = 0;
	bIsCleared = false;
}

void AHSGameMode::OnPlayerScoreChanged(int32 NewPlayerScore)
{
	CurrentScore = NewPlayerScore;

	AHSPlayerController* HSPlayerController = Cast<AHSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (HSPlayerController)
	{
		HSPlayerController->GameScoreChanged(CurrentScore);
	}

	if (CurrentScore >= ClearScore)
	{
		bIsCleared = true;

		if (HSPlayerController)
		{
			HSPlayerController->GameClear();

		}
	}
}

void AHSGameMode::OnPlayerDead()
{
	AHSPlayerController* HSPlayerController = Cast<AHSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (HSPlayerController)
	{
		HSPlayerController->GameOver();
	}
}

bool AHSGameMode::IsGameCleared()
{
	return bIsCleared;
}
