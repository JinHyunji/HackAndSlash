// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/HSCharacterBase.h"
#include "Engine/StreamableManager.h"
#include "Interface/HSCharacterAIInterface.h"
#include "HSCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS(Config = HackAndSlash)
class HACKANDSLASH_API AHSCharacterNonPlayer : public AHSCharacterBase, public IHSCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	AHSCharacterNonPlayer();

protected:
	virtual void PostInitializeComponents() override;

	virtual void SetDead() override;
	void NPCMeshLoadCompleted();

	UPROPERTY(Config)
	TArray<FSoftObjectPath> NPCMeshes;

	TSharedPtr<FStreamableHandle> NPCMeshHandle;


	// AI Section
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
