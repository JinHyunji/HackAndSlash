// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HSItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon = 0,
	Potion,
	Scroll
};

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};
