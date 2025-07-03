// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/HSGameSingleton.h"

DEFINE_LOG_CATEGORY(LogHSGameSingleton);

UHSGameSingleton::UHSGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef (TEXT("/Script/Engine.DataTable'/Game/HackAndSlash/GameData/HSCharacterStatTable.HSCharacterStatTable'"));
	if (DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);

		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FHSCharacterStat*>(Value);
			}
		);
	}

	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
}

UHSGameSingleton& UHSGameSingleton::Get()
{
	UHSGameSingleton* Singleton = CastChecked<UHSGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogHSGameSingleton, Error, TEXT("InValid Game Singleton"));
	return *NewObject<UHSGameSingleton>();
}
