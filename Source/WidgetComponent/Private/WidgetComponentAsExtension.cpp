// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponentAsExtension.h"

#include "WidgetComponentStatics.h"
#include "Blueprint/UserWidget.h"
#include "Macro/AssertionMacros.h"

void UWidgetComponentAsExtension::SetComponentsFiledPath(const TFieldPath<FArrayProperty> InComponentsFiledPath)
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

	WidgetComponentStatics::LinkSoftObjectToRuntimeVariable(this);
}

bool UWidgetComponentAsExtension::RequiresTick() const
{
	return false;
}
