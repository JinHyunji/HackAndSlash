// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/HSPlayerController.h"

void AHSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 시작 시 마우스가 뷰포트 안으로 들어가게 설정
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
	// ======================================
}