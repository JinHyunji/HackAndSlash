// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/HSCharacterNonPlayer.h"
#include "Engine/AssetManager.h"
#include "AI/HSAIController.h"
#include "CharacterStat/HSCharacterStatComponent.h"

AHSCharacterNonPlayer::AHSCharacterNonPlayer()
{
	GetMesh()->SetHiddenInGame(true);

	AIControllerClass = AHSAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AHSCharacterNonPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ensure(NPCMeshes.Num() > 0);
	int32 RandIndex = FMath::RandRange(0, NPCMeshes.Num() - 1);
	NPCMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(NPCMeshes[RandIndex], FStreamableDelegate::CreateUObject(this, &AHSCharacterNonPlayer::NPCMeshLoadCompleted));
}

void AHSCharacterNonPlayer::SetDead()
{
	Super::SetDead();

	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AHSCharacterNonPlayer::NPCMeshLoadCompleted()
{
	if (NPCMeshHandle.IsValid())
	{
		USkeletalMesh* NPCMesh = Cast<USkeletalMesh>(NPCMeshHandle->GetLoadedAsset());
		if (NPCMesh)
		{
			GetMesh()->SetSkeletalMesh(NPCMesh);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	NPCMeshHandle->ReleaseHandle();
}

float AHSCharacterNonPlayer::GetAIPatrolRadius()
{
	return 800.0f;
}

float AHSCharacterNonPlayer::GetAIDetectRange()
{
	return 400.0f;
}

float AHSCharacterNonPlayer::GetAIAttackRange()
{
	return Stat->GetTotalStat().AttackRange + Stat->GetAttackRadius() * 2;
}

float AHSCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.f;
}

void AHSCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AHSCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void AHSCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}