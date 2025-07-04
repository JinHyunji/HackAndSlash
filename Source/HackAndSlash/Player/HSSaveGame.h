// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HSSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UHSSaveGame();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 RetryCount;
	
};
