// Fill out your copyright notice in the Description page of Project Settings.


#include "RemWidgetComponentAsExtension.h"

#include "RemWidgetComponentStatics.h"
#include "Blueprint/UserWidget.h"
#include "Macro/RemAssertionMacros.h"

void URemWidgetComponentAsExtension::SetComponentsFiledPath(const TFieldPath<FArrayProperty>& InComponentsFiledPath)
{
	ComponentsFiledPath = InComponentsFiledPath;
}

FArrayProperty* URemWidgetComponentAsExtension::GetComponentsProperty() const
{
	RemCheckVariable(GetUserWidget(), return {});
	
	return GetComponentsFiledPath().Get(GetUserWidget()->StaticClass());
}

void URemWidgetComponentAsExtension::Initialize()
{
	Super::Initialize();

	Rem::WidgetComponent::LinkSoftObjectToRuntimeVariable(this);
}

bool URemWidgetComponentAsExtension::RequiresTick() const
{
	return false;
}
