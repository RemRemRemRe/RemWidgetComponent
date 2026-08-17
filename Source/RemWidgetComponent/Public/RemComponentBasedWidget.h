// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/RemComponentContainer.h"
#include "Components/RemComponentContainerOwnerInterface.h"

#include "RemComponentBasedWidget.generated.h"

/**
 * A widget hosting reusable behavior components (EC pattern).
 *
 * Components are instanced structs stored in a FRemComponentContainer (RemCommon).
 * The widget self-drives their lifecycle:
 *  - Initialize (after the widget tree is ready) links soft widget references by
 *    name, then initializes the components;
 *  - Destruct uninitializes the components;
 *  - NativeTick dispatches ShouldTick-gated Tick to every component.
 *
 * @note The widget ticks whenever the engine ticks it (BP subclasses tick always);
 *       per-component ticking is gated by FRemComponentBase::ShouldTick.
 */
UCLASS(Blueprintable)
class REMWIDGETCOMPONENT_API URemComponentBasedWidget : public UUserWidget,
    public IRemComponentContainerOwnerInterface
{
    GENERATED_BODY()

#pragma region Data Members

    UPROPERTY(EditAnywhere, Category = "Component")
    FRemComponentContainer Components{};

#pragma endregion Data Members

public:
    virtual bool Initialize() override;

protected:
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

#pragma region IRemComponentContainerOwnerInterface

public:
    virtual Rem::TNotNull<FRemComponentContainer*> GetComponentContainer() override;
    virtual Rem::TNotNull<const FRemComponentContainer*> GetComponentContainer() const override;

#pragma endregion IRemComponentContainerOwnerInterface
};
