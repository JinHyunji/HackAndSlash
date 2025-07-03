// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HSHUDWidget.h"
#include "Interface/HSCharacterHUDInterface.h"
#include "HSHpBarWidget.h"
#include "HSCharacterStatWidget.h"

UHSHUDWidget::UHSHUDWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UHSHUDWidget::UpdateStat(const FHSCharacterStat& BaseStat, const FHSCharacterStat& ModifierStat)
{
	FHSCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void UHSHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	HpBar->UpdateHpBar(NewCurrentHp);
}

void UHSHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<UHSHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<UHSCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);
	
	IHSCharacterHUDInterface* HUDPawn = Cast<IHSCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}
}