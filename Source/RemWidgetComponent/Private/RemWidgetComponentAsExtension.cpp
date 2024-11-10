// Copyright RemRemRemRe, All Rights Reserved.


#include "RemWidgetComponentAsExtension.h"

#include "RemWidgetComponentStatics.h"
#include "Blueprint/UserWidget.h"
#include "Macro/RemAssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemWidgetComponentAsExtension)

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
