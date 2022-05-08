// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponentStatics.h"

#include "WidgetComponentAsExtension.h"
#include "WidgetComponentBase.h"
#include "WidgetComponentLog.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Macro/AssertionMacros.h"
#include "Templates/PropertyHelper.h"
#include "Object/ObjectStatics.h"

namespace WidgetComponentStatics
{
void ForeachUserWidgetComponent(const UUserWidget* UserWidget,
	const TFunctionRef<void(UWidgetComponentBase** MemberPtr, int32 Index)> Predicate)
{
	CheckPointer(UserWidget, return;);
	ForeachUserWidgetComponent(UserWidget->GetExtension<UWidgetComponentAsExtension>(), Predicate);
}

void WidgetComponentStatics::ForeachUserWidgetComponent(const UWidgetComponentAsExtension* Extension,
	const TFunctionRef<void(UWidgetComponentBase** MemberPtr, int32 Index)> Predicate)
{
	CheckPointer(Extension, return;);
	
	const FArrayProperty* ComponentsProperty = Extension->GetComponentsProperty();

	CheckCondition(Common::PropertyHelper::IsPropertyClassChildOf(ComponentsProperty->Inner,
		UWidgetComponentBase::StaticClass()), return;);

	Common::ObjectStatics::ForeachObjectInArray(ComponentsProperty, Extension->GetUserWidget(),
		[&] (void* ObjectMemberPtr, const int32 Index)
	{
		UWidgetComponentBase** MemberPtr = static_cast<UWidgetComponentBase**>(ObjectMemberPtr);

		Predicate(MemberPtr, Index);
	});
}

void AddComponentsToWidgetExtension(const UWidgetComponentAsExtension* Extension)
{
	CheckPointer(Extension, return;);

	CheckPointer(Extension->GetUserWidget(), return;);
	
	if (Extension->GetUserWidget()->IsDesignTime())
	{
		return;
	}
	
	ForeachUserWidgetComponent(Extension,
	[&](UWidgetComponentBase** ObjectMemberPtr, int32)
	{
		UWidgetComponentBase* ComponentBase = *ObjectMemberPtr;
		CheckPointer(ComponentBase, return);
		
		Extension->GetUserWidget()->AddExtension(ComponentBase);
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
	[&] (UWidgetComponentBase** ObjectMemberPtr, int32)
	{
		const UWidgetComponentBase* Component = *ObjectMemberPtr;
		CheckPointer(Component, return);
		
		Common::PropertyHelper::IteratePropertiesOfType<FSoftObjectProperty>(Component->GetClass(), Component,
		[&] (const FProperty* InProperty, const void* InContainer, int32,
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

UWidgetComponentAsExtension* GetOrAddWidgetComponentAsExtension(UUserWidget* UserWidget)
{
	CheckPointer(UserWidget, return {};);

	UWidgetComponentAsExtension* Extension = UserWidget->GetExtension<UWidgetComponentAsExtension>();
	if (!Extension)
	{
		Extension = UserWidget->AddExtension<UWidgetComponentAsExtension>();
	}

	CheckPointer(Extension);

	return Extension;
}
	
}

	