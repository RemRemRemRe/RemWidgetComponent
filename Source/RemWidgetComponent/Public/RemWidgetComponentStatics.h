// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemWidgetComponentStatics.generated.h"

class UWidgetComponentBase;
class UWidgetComponentAsExtension;

/**
 * 
 */
UCLASS()
class REMWIDGETCOMPONENT_API UWidgetComponentStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
};

namespace Rem::WidgetComponent
{
	REMWIDGETCOMPONENT_API
	void ForeachUserWidgetComponent(const UUserWidget* UserWidget,
		TFunctionRef<void(UWidgetComponentBase** MemberPtr, int32 Index)> Predicate);

	REMWIDGETCOMPONENT_API
	void ForeachUserWidgetComponent(const UWidgetComponentAsExtension* Extension,
		TFunctionRef<void(UWidgetComponentBase** MemberPtr, int32 Index)> Predicate);

	REMWIDGETCOMPONENT_API
	void AddComponentsToWidgetExtension(const UWidgetComponentAsExtension* Extension);
	
	REMWIDGETCOMPONENT_API
	void LinkSoftObjectToRuntimeVariable(const UWidgetComponentAsExtension* Extension);

	REMWIDGETCOMPONENT_API
	UWidgetComponentAsExtension* GetOrAddWidgetComponentAsExtension(UUserWidget* UserWidget);
	
}
