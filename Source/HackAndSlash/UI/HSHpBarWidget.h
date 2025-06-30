// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HSUserWidget.h"
#include "HSHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSHpBarWidget : public UHSUserWidget
{
	GENERATED_BODY()
	
public:
	UHSHpBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
