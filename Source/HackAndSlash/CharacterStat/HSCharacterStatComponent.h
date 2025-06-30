// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HSCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /* Current Hp */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HACKANDSLASH_API UHSCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;

	// Sets default values for this component's properties
	UHSCharacterStatComponent();

	FORCEINLINE float GetMaxHp() { return MaxHp; }
	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetHp(float NewHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float MaxHp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

};
