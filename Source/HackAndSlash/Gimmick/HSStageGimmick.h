// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HSStageGimmick.generated.h"

DECLARE_DELEGATE(FOnStageChangeDelegate);
USTRUCT(BlueprintType)
struct FStageChangedDelegateWrapper
{
	GENERATED_BODY()
	FStageChangedDelegateWrapper() {}
	FStageChangedDelegateWrapper(const FOnStageChangeDelegate& InDelegate) : StageDelegate(InDelegate) {}
	FOnStageChangeDelegate StageDelegate;
};


UENUM(BlueprintType)
enum class EStageState : uint8
{
	READY = 0,
	FIGHT,
	REWARD,
	NEXT
};

UCLASS()
class HACKANDSLASH_API AHSStageGimmick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHSStageGimmick();

	FORCEINLINE int32 GetStageNum() const { return CurrentStageNum; }
	FORCEINLINE void SetStageNum(int32 NewStageNum) { CurrentStageNum = NewStageNum; }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Stage Section
	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> Stage;

	UPROPERTY(VisibleAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UBoxComponent> StageTrigger;

	UFUNCTION()
	void OnStageTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);


	// Gate Section
	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
	TMap<FName, TObjectPtr<class UStaticMeshComponent>> Gates;

	UPROPERTY(VisibleAnywhere, Category = Gate, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UBoxComponent>> GateTriggers;

	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	void OpenAllGates();
	void CloseAllGates();


	// State Section
	UPROPERTY(EditAnywhere, Category = Stage, meta = (AllowPrivateAccess = "true"))
	EStageState CurrentState;

	void SetState(EStageState InNewState);

	UPROPERTY()
	TMap<EStageState, FStageChangedDelegateWrapper> StageChangeActions;

	void SetReady();
	void SetFight();
	void SetChooseReward();
	void SetChooseNext();


	// Fight Section
	UPROPERTY(EditAnywhere, Category = Fight, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AHSCharacterNonPlayer> OpponentClass;

	UPROPERTY(EditAnywhere, Category = Fight, meta = (AllowPrivateAccess = "true"))
	float OpponentSpawnTime;

	UFUNCTION()
	void OnOpponentDestroyed(AActor* DestroyedActor);

	FTimerHandle OpponentTimerHandle;
	void OnOpponentSpawn();


	// Reward Section
	UPROPERTY(VisibleAnywhere, Category = Reward, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AHSItemBox> RewardBoxClass;

	UPROPERTY(VisibleAnywhere, Category = Reward, meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<class AHSItemBox>> RewardBoxes;

	TMap<FName, FVector> RewardBoxLocations;

	UFUNCTION()
	void OnRewardTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	void SpawnRewardBoxes();


	// Stat Section
	UPROPERTY(VisibleInstanceOnly, Category = Stat, meta = (AllowPrivateAccess = "true"))
	int32 CurrentStageNum;

};
