// Fill out your copyright notice in the Description page of Project Settings.


#include "RemComponentBasedWidget.h"

#include "RemWidgetComponentAsExtension.h"
#include "RemWidgetComponentStatics.h"
#include "Macro/RemAssertionMacros.h"

TArray<UWidgetComponentBase*> UComponentBasedWidget::GetComponents() const
{
	return Components;
}

TArray<TObjectPtr<UWidgetComponentBase>> UComponentBasedWidget::GetComponentsObjectPtr() const
{
	return Components;
}

FArrayProperty* UComponentBasedWidget::GetComponentsProperty() const
{
	static FArrayProperty* Prop = FindFieldChecked<FArrayProperty>(GetClass(),
		GET_MEMBER_NAME_CHECKED(UComponentBasedWidget, Components));
	return Prop;
}

bool UComponentBasedWidget::Initialize()
{
	if (UWidgetComponentAsExtension* Component = Rem::WidgetComponent::GetOrAddWidgetComponentAsExtension(this);
		ensureAlways(Component))
	{
		Component->SetComponentsFiledPath(GetComponentsProperty());

		Rem::WidgetComponent::AddComponentsToWidgetExtension(Component);
	}

	return Super::Initialize();
}

#if WITH_EDITOR

void UComponentBasedWidget::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

	// CDO will not run Initialize, so add the component here
	UWidgetComponentAsExtension* Extension = Rem::WidgetComponent::GetOrAddWidgetComponentAsExtension(this);;
	CheckPointer(Extension, return);

	Extension->SetFlags(RF_Transient);

	Extension->SetComponentsFiledPath(GetComponentsProperty());
}

#endif
