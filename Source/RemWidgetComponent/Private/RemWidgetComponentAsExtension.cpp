// Fill out your copyright notice in the Description page of Project Settings.


#include "RemWidgetComponentAsExtension.h"

#include "RemWidgetComponentStatics.h"
#include "Blueprint/UserWidget.h"
#include "Macro/RemAssertionMacros.h"

void UWidgetComponentAsExtension::SetComponentsFiledPath(const TFieldPath<FArrayProperty>& InComponentsFiledPath)
{
	ComponentsFiledPath = InComponentsFiledPath;
}

FArrayProperty* UWidgetComponentAsExtension::GetComponentsProperty() const
{
	CheckPointer(GetUserWidget(), return {});
	
	return GetComponentsFiledPath().Get(GetUserWidget()->StaticClass());
}

void UWidgetComponentAsExtension::Initialize()
{
	Super::Initialize();

	Rem::WidgetComponent::LinkSoftObjectToRuntimeVariable(this);
}

bool UWidgetComponentAsExtension::RequiresTick() const
{
	return false;
}
