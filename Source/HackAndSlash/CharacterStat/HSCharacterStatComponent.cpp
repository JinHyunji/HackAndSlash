// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/HSCharacterStatComponent.h"

// Sets default values for this component's properties
UHSCharacterStatComponent::UHSCharacterStatComponent()
{
	MaxHp = 200.f;
	SetHp(MaxHp);
}

float UHSCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		// Dead
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

// Called when the game starts
void UHSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHp = MaxHp;
}

void UHSCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}
