// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSCharacterNonPlayer.h"

AHSCharacterNonPlayer::AHSCharacterNonPlayer()
{

}

void AHSCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}