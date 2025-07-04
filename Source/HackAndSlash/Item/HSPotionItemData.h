// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HSItemData.h"
#include "GameData/HSCharacterStat.h"
#include "HSPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSPotionItemData : public UHSItemData
{
	GENERATED_BODY()

public:
	UHSPotionItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("HSItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, Category = Hp)
	float HealAmount;


};
