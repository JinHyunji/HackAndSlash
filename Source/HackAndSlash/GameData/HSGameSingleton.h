// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HSCharacterStat.h"
#include "HSGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogHSGameSingleton, Error, All);

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UHSGameSingleton();
	static UHSGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FHSCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FHSCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FHSCharacterStat> CharacterStatTable;
};
