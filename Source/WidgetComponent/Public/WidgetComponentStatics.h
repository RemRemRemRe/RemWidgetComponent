// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetComponentStatics.generated.h"

class UWidgetComponentBase;
class UWidgetComponentAsExtension;

/**
 * 
 */
UCLASS()
class WIDGETCOMPONENT_API UWidgetComponentStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
};

namespace WidgetComponentStatics
{
	WIDGETCOMPONENT_API
	void ForeachUserWidgetComponent(const UUserWidget* UserWidget,
		TFunctionRef<void(UWidgetComponentBase** MemberPtr, int32 Index)> Predicate);

	WIDGETCOMPONENT_API
	void ForeachUserWidgetComponent(const UWidgetComponentAsExtension* Extension,
		TFunctionRef<void(UWidgetComponentBase** MemberPtr, int32 Index)> Predicate);

	WIDGETCOMPONENT_API
	void AddComponentsToWidgetExtension(const UWidgetComponentAsExtension* Extension);
	
	WIDGETCOMPONENT_API
	void LinkSoftObjectToRuntimeVariable(const UWidgetComponentAsExtension* Extension);

	WIDGETCOMPONENT_API
	UWidgetComponentAsExtension* GetOrAddWidgetComponentAsExtension(UUserWidget* UserWidget);
	
}
