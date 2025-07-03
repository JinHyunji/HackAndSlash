// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/HSCharacterStat.h"
#include "HSHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UHSHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FHSCharacterStat& BaseStat, const FHSCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TObjectPtr<class UHSHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class UHSCharacterStatWidget> CharacterStat;
};
