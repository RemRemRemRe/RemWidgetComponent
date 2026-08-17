// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "RemComponentBasedWidget.h"

#include "RemWidgetComponentStatics.h"
#include "RemWidgetComponentStats.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RemComponentBasedWidget)

bool URemComponentBasedWidget::Initialize()
{
    if (!Super::Initialize())
    {
        return false;
    }

    if (IsDesignTime())
    {
        return true;
    }

    // The widget tree is fully built after Super::Initialize(): link soft widget
    // references by name first, then initialize the components (they may resolve
    // their targets inside Initialize).
    Rem::WidgetComponent::LinkComponentsToWidgetTree(*this, Components);
    Components.TryInitialize(*this);

    return true;
}

void URemComponentBasedWidget::NativeDestruct()
{
    Components.TryUninitialize();

    Super::NativeDestruct();
}

void URemComponentBasedWidget::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    SCOPE_CYCLE_COUNTER(STAT_WidgetComponent);

    if (!Components.IsInitialized())
    {
        return;
    }

    Components.ForEachComponent<FRemComponentBase>(
        [&](FRemComponentBase& Component, const int32 Index, const UScriptStruct&)
        {
            if (const FRemComponentBase::FContext Context{.OwnerInstance = &Components, .ComponentIndex = Index};
                Component.ShouldTick(Context))
            {
                Component.Tick(Context, InDeltaTime);
            }
        });
}

Rem::TNotNull<FRemComponentContainer*> URemComponentBasedWidget::GetComponentContainer()
{
    return &Components;
}

Rem::TNotNull<const FRemComponentContainer*> URemComponentBasedWidget::GetComponentContainer() const
{
    return &Components;
}
