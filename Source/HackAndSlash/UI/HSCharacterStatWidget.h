// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/HSCharacterStat.h"
#include "HSCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void UpdateStat(const FHSCharacterStat& BaseStat, const FHSCharacterStat& ModifierStat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;
};
