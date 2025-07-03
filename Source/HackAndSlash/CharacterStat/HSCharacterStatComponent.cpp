// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/HSCharacterStatComponent.h"
#include "GameData/HSGameSingleton.h"

// Sets default values for this component's properties
UHSCharacterStatComponent::UHSCharacterStatComponent()
{
	CurrentLevel = 1;
	AttackRadius = 50.f;

	bWantsInitializeComponent = true; // 이 변수를 true로 설정해야 InitializeComponent() 호출됨
}

void UHSCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, UHSGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(UHSGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.f);
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

void UHSCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
}

void UHSCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}
