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
#if WITH_EDITOR
	WidgetComponentStatics::RemoveWidgetComponentAsExtension(this);
#endif
	
	UWidgetComponentAsExtension* Component = AddExtension<UWidgetComponentAsExtension>();
	CheckPointer(Component, return {});

	Component->SetComponentsFiledPath(GetComponentsProperty());

	WidgetComponentStatics::AddComponentsToWidgetExtension(Component);
	
	return Super::Initialize();
}

#if WITH_EDITOR

void UComponentBasedWidget::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);

	// CDO will not run Initialize, so add the component here

	WidgetComponentStatics::RemoveWidgetComponentAsExtension(this);
	
	UWidgetComponentAsExtension* Extension = AddExtension<UWidgetComponentAsExtension>();
	CheckPointer(Extension, return);

	Extension->SetFlags(RF_Transient);
	
	Extension->SetComponentsFiledPath(GetComponentsProperty());
}

#endif
