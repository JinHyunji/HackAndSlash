// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HSHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/HSCharacterWidgetInterface.h"

UHSHpBarWidget::UHSHpBarWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MaxHp = -1.f;
}

void UHSHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbHpBar")));
	ensure(HpProgressBar);

	IHSCharacterWidgetInterface* CharacterWidget = Cast<IHSCharacterWidgetInterface>(OwningActor);
	if (CharacterWidget)
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UHSHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp / MaxHp);
	}

}
