// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponentStatics.h"

#include "WidgetComponentAsExtension.h"
#include "WidgetComponentBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Macro/AssertionMacros.h"
#include "Templates/PropertyHelper.h"
#include "Object/ObjectStatics.h"

namespace WidgetComponentStatics
{

void WidgetComponentStatics::ForeachUserWidgetComponent(const UWidgetComponentAsExtension* Extension,
	const TFunctionRef<void(UWidgetComponentBase*)> Predicate)
{
	const FArrayProperty* ComponentsProperty = Extension->GetComponentsProperty();

	CheckCondition(Common::PropertyHelper::IsPropertyClassChildOf(ComponentsProperty->Inner,
		UWidgetComponentBase::StaticClass()), return;);

	Common::ObjectStatics::ForeachObjectInArray(ComponentsProperty, Extension->GetUserWidget(),
		[&] (UObject* Object)
	{
		UWidgetComponentBase* Component = Cast<UWidgetComponentBase>(Object);
		CheckPointer(Component, return;);

		Predicate(Component);
	});
}

void AddComponentsToWidgetExtension(const UWidgetComponentAsExtension* Extension)
{
	ForeachUserWidgetComponent(Extension,
	[&](UWidgetComponentBase* Component)
	{
		Extension->GetUserWidget()->AddExtension(Component);
	});
}

void WidgetComponentStatics::LinkSoftObjectToRuntimeVariable(const UWidgetComponentAsExtension* Extension)
{
	CheckPointer(Extension, return;);

	CheckPointer(Extension->GetUserWidget(), return;);

	if (Extension->GetUserWidget()->IsDesignTime())
	{
		return;
	}

	TMap<FName, UWidget*> NamedWidgetMap;
	Extension->GetUserWidget()->WidgetTree->ForEachWidget([&] (UWidget* Widget)
	{
		CheckPointer(Widget, return);

		NamedWidgetMap.FindOrAdd(Widget->GetFName(), Widget);
	});

	ForeachUserWidgetComponent(Extension,
	[&] (const UWidgetComponentBase* Component)
	{
		Common::PropertyHelper::IteratePropertiesOfType<FSoftObjectProperty>(Component->GetClass(), Component, [&]
		(const FProperty* InProperty, const void* InContainer, int32,
		const FString&, const FString&, const FString&, int32, int32)
		{
			const FSoftObjectProperty* SoftObjectProperty = CastField<FSoftObjectProperty>(const_cast<FProperty*>(InProperty));
			CheckPointer(SoftObjectProperty, return);
			
			FSoftObjectPtr* SoftObjectPtr = SoftObjectProperty->GetPropertyValuePtr_InContainer(const_cast<void*>(InContainer));
			CheckPointer(SoftObjectPtr, return);

			if (SoftObjectPtr->IsNull())
			{
				return;
			}
			
			if (UWidget** Value = NamedWidgetMap.Find(*Common::GetObjectNameFromSoftObjectPath(SoftObjectPtr->ToSoftObjectPath())))
			{
				// link SoftObjectPtr to the runtime variable
				*SoftObjectPtr = *Value;
				return;
			}

			CheckCondition(false);
		});
	});
}
	
}

	