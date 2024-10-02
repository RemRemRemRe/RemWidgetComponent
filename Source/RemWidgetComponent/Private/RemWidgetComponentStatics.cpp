// Copyright RemRemRemRe, All Rights Reserved.


#include "RemWidgetComponentStatics.h"

#include "RemWidgetComponentAsExtension.h"
#include "RemWidgetComponentBase.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Macro/RemAssertionMacros.h"
#include "Templates/RemPropertyHelper.h"
#include "Templates/RemIteratePropertiesOfType.h"
#include "Object/RemObjectStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemWidgetComponentStatics)

namespace Rem::WidgetComponent
{
void ForeachUserWidgetComponent(const UUserWidget* UserWidget,
	const TFunctionRef<void(URemWidgetComponentBase** MemberPtr, int32 Index)>& Predicate)
{
	RemCheckVariable(UserWidget, return;);
	ForeachUserWidgetComponent(UserWidget->GetExtension<URemWidgetComponentAsExtension>(), Predicate);
}

void ForeachUserWidgetComponent(const URemWidgetComponentAsExtension* Extension,
	const TFunctionRef<void(URemWidgetComponentBase** MemberPtr, int32 Index)>& Predicate)
{
	RemCheckVariable(Extension, return;);

	const FArrayProperty* ComponentsProperty = Extension->GetComponentsProperty();

	RemCheckCondition(Rem::Property::IsPropertyClassChildOf(ComponentsProperty->Inner,
		URemWidgetComponentBase::StaticClass()), return;);

	Object::ForeachObjectInArray(ComponentsProperty, Extension->GetOuterUUserWidget(),
		[&] (void* ObjectMemberPtr, const int32 Index)
	{
		URemWidgetComponentBase** MemberPtr = static_cast<URemWidgetComponentBase**>(ObjectMemberPtr);

		Predicate(MemberPtr, Index);
	});
}

void AddComponentsToWidgetExtension(const URemWidgetComponentAsExtension* Extension)
{
	RemCheckVariable(Extension, return;);

	UUserWidget* UserWidget = Extension->GetOuterUUserWidget();
	RemCheckVariable(UserWidget, return;);

	if (UserWidget->IsDesignTime())
	{
		return;
	}

	ForeachUserWidgetComponent(Extension,
	[&](URemWidgetComponentBase** ObjectMemberPtr, int32)
	{
		URemWidgetComponentBase* ComponentBase = *ObjectMemberPtr;
		RemCheckVariable(ComponentBase, return);

		// TODO : refactor to instanced struct
		//UserWidget->AddExtension(ComponentBase);
	});
}

void LinkSoftObjectToRuntimeVariable(const URemWidgetComponentAsExtension* Extension)
{
	RemCheckVariable(Extension, return;);

	UUserWidget* UserWidget = Extension->GetOuterUUserWidget();
	RemCheckVariable(UserWidget, return;);

	if (UserWidget->IsDesignTime())
	{
		return;
	}

	TMap<FName, UWidget*> NamedWidgetMap;
	UserWidget->WidgetTree->ForEachWidget([&] (UWidget* Widget)
	{
		RemCheckVariable(Widget, return);

		NamedWidgetMap.FindOrAdd(Widget->GetFName(), Widget);
	});

	ForeachUserWidgetComponent(Extension,
	[&] (URemWidgetComponentBase** ObjectMemberPtr, int32)
	{
		const URemWidgetComponentBase* Component = *ObjectMemberPtr;
		RemCheckVariable(Component, return);

		Property::IteratePropertiesOfType<FSoftObjectProperty>(Component->GetClass(), Component,
		[&] (const FProperty* InProperty, const void* InContainer)
		{
			auto* SoftObjectProperty = CastField<const FSoftObjectProperty>(InProperty);
			RemCheckVariable(SoftObjectProperty, return);

			auto* SoftObjectPtr = SoftObjectProperty->GetPropertyValuePtr_InContainer(const_cast<void*>(InContainer));
			RemCheckVariable(SoftObjectPtr, return);

			if (SoftObjectPtr->IsNull())
			{
				return;
			}

			if (UWidget** Value = NamedWidgetMap.Find(*GetObjectNameFromSoftObjectPath(SoftObjectPtr->ToSoftObjectPath())))
			{
				// link SoftObjectPtr to the runtime variable
				*SoftObjectPtr = *Value;
				return;
			}

			RemCheckCondition(false);
		});
	});
}

URemWidgetComponentAsExtension* GetOrAddWidgetComponentAsExtension(UUserWidget* UserWidget)
{
	RemCheckVariable(UserWidget, return {};);

	URemWidgetComponentAsExtension* Extension = UserWidget->GetExtension<URemWidgetComponentAsExtension>();
	if (!Extension)
	{
		Extension = UserWidget->AddExtension<URemWidgetComponentAsExtension>();
	}

	RemCheckVariable(Extension);

	return Extension;
}

}
