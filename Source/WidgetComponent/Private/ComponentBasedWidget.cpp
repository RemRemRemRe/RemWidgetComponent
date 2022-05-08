// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentBasedWidget.h"

#include "WidgetComponentAsExtension.h"
#include "WidgetComponentStatics.h"
#include "Macro/AssertionMacros.h"

TArray<UWidgetComponentBase*> UComponentBasedWidget::GetComponents() const
{
	return Components;
}

TArray<TObjectPtr<UWidgetComponentBase>> UComponentBasedWidget::GetComponentsPtr() const
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
	if (UWidgetComponentAsExtension* Component = WidgetComponentStatics::GetOrAddWidgetComponentAsExtension(this);
		ensureAlways(Component))
	{
		Component->SetComponentsFiledPath(GetComponentsProperty());

		WidgetComponentStatics::AddComponentsToWidgetExtension(Component);
	}
	
	return Super::Initialize();
}

#if WITH_EDITOR

void UComponentBasedWidget::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

	// CDO will not run Initialize, so add the component here
	UWidgetComponentAsExtension* Extension = WidgetComponentStatics::GetOrAddWidgetComponentAsExtension(this);;
	CheckPointer(Extension, return);

	Extension->SetComponentsFiledPath(GetComponentsProperty());
}

#endif
