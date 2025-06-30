// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HSWidgetComponent.h"
#include "HSUserWidget.h"

void UHSWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UHSUserWidget* HSUserWidget = Cast<UHSUserWidget>(GetWidget());
	if (HSUserWidget)
	{
		HSUserWidget->SetOwningActor(GetOwner());
	}
}