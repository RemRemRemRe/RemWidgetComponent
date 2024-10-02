// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RemWidgetComponentStatics.generated.h"

class UUserWidget;
class URemWidgetComponentBase;
class URemWidgetComponentAsExtension;


UCLASS()
class REMWIDGETCOMPONENT_API URemWidgetComponentStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
};

namespace Rem::WidgetComponent
{
	REMWIDGETCOMPONENT_API
	void ForeachUserWidgetComponent(const UUserWidget* UserWidget,
		const TFunctionRef<void(URemWidgetComponentBase** MemberPtr, int32 Index)>& Predicate);

	REMWIDGETCOMPONENT_API
	void ForeachUserWidgetComponent(const URemWidgetComponentAsExtension* Extension,
		const TFunctionRef<void(URemWidgetComponentBase** MemberPtr, int32 Index)>& Predicate);

	REMWIDGETCOMPONENT_API
	void AddComponentsToWidgetExtension(const URemWidgetComponentAsExtension* Extension);

	REMWIDGETCOMPONENT_API
	void LinkSoftObjectToRuntimeVariable(const URemWidgetComponentAsExtension* Extension);

	REMWIDGETCOMPONENT_API
	URemWidgetComponentAsExtension* GetOrAddWidgetComponentAsExtension(UUserWidget* UserWidget);

}
