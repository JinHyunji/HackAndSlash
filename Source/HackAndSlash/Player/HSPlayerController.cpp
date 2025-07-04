// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HSPlayerController.h"
#include "UI/HSHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HSSaveGame.h"

DEFINE_LOG_CATEGORY(LogHSPlayerController);

AHSPlayerController::AHSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UHSHUDWidget> HSHUDWidgetRef(TEXT("/Game/HackAndSlash/UI/WBP_HSHUD.WBP_HSHUD_C"));
	if (HSHUDWidgetRef.Class)
	{
		HSHUDWidgetClass = HSHUDWidgetRef.Class;
	}
}

void AHSPlayerController::GameScoreChanged(int32 NewScore)
{
	K2_OnScoreChanged(NewScore);
}

void AHSPlayerController::GameClear()
{
	K2_OnGameClear();
}

void AHSPlayerController::GameOver()
{
	K2_OnGameOver();
	
	if (!UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("Player0"), 0))
	{
		UE_LOG(LogHSPlayerController, Error, TEXT("Save Game Error!"));
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}

void AHSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 마우스가 뷰포트 안으로 들어가게 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	// ======================================
	
	SaveGameInstance = Cast<UHSSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Player0"), 0));
	if (SaveGameInstance)
	{
		SaveGameInstance->RetryCount++;
	}
	else
	{
		SaveGameInstance = NewObject<UHSSaveGame>();
	}

	K2_OnGameRetryCount(SaveGameInstance->RetryCount);
}