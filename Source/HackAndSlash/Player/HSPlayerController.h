// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HSPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API AHSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AHSPlayerController();

protected:
	virtual void BeginPlay() override;

// HUD Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HUD)
	TSubclassOf<class UHSHUDWidget> HSHUDWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD)
	TObjectPtr<class UHSHUDWidget> HSHUDWidget;

};
