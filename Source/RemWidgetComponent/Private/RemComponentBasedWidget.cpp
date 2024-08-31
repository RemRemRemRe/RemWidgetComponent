// Copyright RemRemRemRe, All Rights Reserved.


#include "RemComponentBasedWidget.h"

#include "RemWidgetComponentAsExtension.h"
#include "RemWidgetComponentStatics.h"
#include "Macro/RemAssertionMacros.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentBasedWidget)

bool URemComponentBasedWidget::Initialize()
{
	if (URemWidgetComponentAsExtension* Component = Rem::WidgetComponent::GetOrAddWidgetComponentAsExtension(this);
		ensureAlways(Component))
	{
		Component->SetComponentsFiledPath(GetComponentsProperty());

		Rem::WidgetComponent::AddComponentsToWidgetExtension(Component);
	}

	return Super::Initialize();
}

#if WITH_EDITOR

void URemComponentBasedWidget::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

	// CDO will not run Initialize, so add the component here
	URemWidgetComponentAsExtension* Extension = Rem::WidgetComponent::GetOrAddWidgetComponentAsExtension(this);;
	RemCheckVariable(Extension, return);

	Extension->SetComponentsFiledPath(GetComponentsProperty());
}

#endif

TArray<URemWidgetComponentBase*> URemComponentBasedWidget::GetComponents() const
{
	return Components;
}

TArray<TObjectPtr<URemWidgetComponentBase>> URemComponentBasedWidget::GetComponentsObjectPtr() const
{
	return Components;
}

FArrayProperty* URemComponentBasedWidget::GetComponentsProperty() const
{
	static FArrayProperty* Prop = FindFieldChecked<FArrayProperty>(GetClass(),
		FName{GET_MEMBER_NAME_STRING_VIEW_CHECKED(URemComponentBasedWidget, Components)});
	return Prop;
}
