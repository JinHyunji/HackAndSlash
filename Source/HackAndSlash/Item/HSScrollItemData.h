// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HSItemData.h"
#include "GameData/HSCharacterStat.h"
#include "HSScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSScrollItemData : public UHSItemData
{
	GENERATED_BODY()

public:
	UHSScrollItemData();

	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("HSItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, Category = Stat)
	FHSCharacterStat BaseStat;
};
