// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HSAIController.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API AHSAIController : public AAIController
{
	GENERATED_BODY()

public:
	AHSAIController();

	void RunAI();
	void StopAI();

protected:
	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	TObjectPtr<class UBlackboardData> BBAsset;
	
	UPROPERTY()
	TObjectPtr<class UBehaviorTree> BTAsset;
	
};
