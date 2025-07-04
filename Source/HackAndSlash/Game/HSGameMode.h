// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/HSGameInterface.h"
#include "HSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API AHSGameMode : public AGameModeBase, public IHSGameInterface
{
	GENERATED_BODY()
	
public:
	AHSGameMode();

	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) override;
	virtual void OnPlayerDead() override;
	virtual bool IsGameCleared() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Game)
	int32 ClearScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	int32 CurrentScore;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Game)
	uint8 bIsCleared : 1;

};
