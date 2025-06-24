// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSCharacterBase.h"
#include "HSCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API AHSCharacterNonPlayer : public AHSCharacterBase
{
	GENERATED_BODY()
	
public:
	AHSCharacterNonPlayer();

protected:
	virtual void SetDead() override;
};
