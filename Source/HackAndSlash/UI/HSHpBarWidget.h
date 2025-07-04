// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSUserWidget.h"
#include "GameData/HSCharacterStat.h"
#include "HSHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSHpBarWidget : public UHSUserWidget
{
	GENERATED_BODY()
	
public:
	UHSHpBarWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateStat(const FHSCharacterStat& BaseStat, const FHSCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
};
