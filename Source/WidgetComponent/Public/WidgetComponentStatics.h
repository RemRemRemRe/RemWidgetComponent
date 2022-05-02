// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	void ForeachUserWidgetComponent(const UWidgetComponentAsExtension* Extension,
		TFunctionRef<void(UWidgetComponentBase*)> Predicate);

	void AddComponentsToWidgetExtension(const UWidgetComponentAsExtension* Extension);
	void LinkSoftObjectToRuntimeVariable(const UWidgetComponentAsExtension* Extension);
}
