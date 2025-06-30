// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/HSItemData.h"
#include "HSWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSWeaponItemData : public UHSItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> WeaponMesh;

};
