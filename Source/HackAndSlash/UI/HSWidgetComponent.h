// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HSWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class HACKANDSLASH_API UHSWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

protected:
	virtual void InitWidget() override;

};
