// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/HSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HSAI.h"

AHSAIController::AHSAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/HackAndSlash/AI/BB_HSCharacter.BB_HSCharacter'"));
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/HackAndSlash/AI/BT_HSCharacter.BT_HSCharacter'"));
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

void AHSAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		bool RunResult = RunBehaviorTree(BTAsset);
		ensure(RunResult);
	}
}

void AHSAIController::StopAI()
{
	UBehaviorTreeComponent* BehaviorPtr = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorPtr)
	{
		BehaviorPtr->StopTree();
	}
}

void AHSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}