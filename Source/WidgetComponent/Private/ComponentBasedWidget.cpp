// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentBasedWidget.h"

TArray<TObjectPtr<UWidgetComponentBase>> UComponentBasedWidget::GetComponents() const
{
	return Components;
}

bool UComponentBasedWidget::Initialize()
{
	return Super::Initialize();
}
