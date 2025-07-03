// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HSItemData.h"
#include "GameData/HSCharacterStat.h"
#include "HSWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSWeaponItemData : public UHSItemData
{
	GENERATED_BODY()

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("HSItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

	UPROPERTY(EditAnywhere, Category = Weapon)
	FHSCharacterStat ModifierStat;
};
