// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemWidgetComponentStatics.h"

#include "Components/RemComponentContainer.h"
#include "Components/RemComponentContainer.inl"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Macro/RemAssertionMacros.h"
#include "RemMisc.h"
#include "Templates/RemIteratePropertiesOfType.h"

namespace Rem::WidgetComponent
{
void LinkComponentsToWidgetTree(const UUserWidget& UserWidget, FRemComponentContainer& Components)
{
    if (UserWidget.IsDesignTime())
    {
        return;
    }

    UWidgetTree* WidgetTree = UserWidget.WidgetTree;
    RemCheckVariable(WidgetTree, return;);

    TMap<FName, UWidget*> NamedWidgetMap;
    WidgetTree->ForEachWidget([&](UWidget* Widget)
    {
        RemCheckVariable(Widget, return);

        NamedWidgetMap.FindOrAdd(Widget->GetFName(), Widget);
    });

    Components.ForEachComponent<FRemComponentBase>(
        [&](FRemComponentBase& Component, const int32, const UScriptStruct& ScriptStruct)
        {
            Rem::Property::IteratePropertiesOfType<FSoftObjectProperty>(Rem::MakeNotNull(&ScriptStruct),
                Rem::MakeNotNull(&Component),
                [&](const Rem::TNotNull<const FSoftObjectProperty*> SoftObjectProperty, void* InValuePtr)
                {
                    // InValuePtr already points at the property value (the TSoftObjectPtr
                    // instance); do NOT re-derive it with GetPropertyValuePtr_InContainer —
                    // that would add the property offset a second time.
                    auto* SoftObjectPtr = static_cast<TSoftObjectPtr<UWidget>*>(InValuePtr);
                    RemCheckVariable(SoftObjectPtr, return);

                    if (SoftObjectPtr->IsNull())
                    {
                        return;
                    }

                    if (UWidget** Value = NamedWidgetMap.Find(
                        *GetObjectNameFromSoftObjectPath(SoftObjectPtr->ToSoftObjectPath())))
                    {
                        // Link the soft reference to the runtime widget: the TSoftObjectPtr
                        // caches it in its built-in weak pointer from now on.
                        *SoftObjectPtr = *Value;
                        return;
                    }

                    RemCheckCondition(false);
                });
        });
}
}
