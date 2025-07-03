// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HSPlayerController.h"
#include "UI/HSHUDWidget.h"

AHSPlayerController::AHSPlayerController()
{
	static ConstructorHelpers::FClassFinder<UHSHUDWidget> HSHUDWidgetRef(TEXT("/Game/HackAndSlash/UI/WBP_HSHUD.WBP_HSHUD_C"));
	if (HSHUDWidgetRef.Class)
	{
		HSHUDWidgetClass = HSHUDWidgetRef.Class;
	}
}

void AHSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 마우스가 뷰포트 안으로 들어가게 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	// ======================================

	HSHUDWidget = CreateWidget<UHSHUDWidget>(this, HSHUDWidgetClass);
	if (HSHUDWidget)
	{
		HSHUDWidget->AddToViewport();
	}
}