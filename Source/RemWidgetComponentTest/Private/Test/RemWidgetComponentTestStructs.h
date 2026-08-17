// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "RemComponentBasedWidget.h"
#include "Components/RemComponentContainer.h"

#include "RemWidgetComponentTestStructs.generated.h"

class UTextBlock;

// Test-only USTRUCT live in a namespace (bAllowUETypesInNamespaces, set by
// RemSharedModuleRules::Apply): they are internal to the test module and must not
// pollute the global scope.
//
// UHT restriction: USTRUCT/UPROPERTY must not be wrapped in #if WITH_DEV_AUTOMATION_TESTS
// (only WITH_EDITORONLY_DATA is allowed). The structs are therefore unguarded; they are
// only referenced from the guarded spec .cpp, so non-test builds carry no test code.

namespace Rem::WidgetComponent::Private
{
/** Plain counters shared by the tracked test components; live in the test binary only. */
struct FRemWidgetComponentTestCounters
{
    static TArray<int32> InitializeOrder;
    static TArray<int32> UninitializeOrder;
    static TArray<int32> TickOrder;
    static TArray<float> TickDeltas;
    static bool bLinkedBeforeInitialize;

    static void Reset();
};

/** Ticking component: records lifecycle calls and whether its soft ref was linked first. */
USTRUCT()
struct FRemWidgetComponentTestTicking : public FRemComponentBase
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Value{};

    UPROPERTY()
    TSoftObjectPtr<UTextBlock> Text;

    virtual void Initialize(const FContext& Context) override;
    virtual bool ShouldTick(const FContext& Context) const override;
    virtual void Tick(const FContext& Context, float DeltaSeconds) override;
    virtual void Uninitialize(const FContext& Context) override;
};

/** Non-ticking component: records lifecycle calls only. */
USTRUCT()
struct FRemWidgetComponentTestIdle : public FRemComponentBase
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    virtual void Initialize(const FContext& Context) override;
    virtual bool ShouldTick(const FContext& Context) const override;
    virtual void Tick(const FContext& Context, float DeltaSeconds) override;
    virtual void Uninitialize(const FContext& Context) override;
};

/** Component with container-typed soft references: exercises recursive property iteration. */
USTRUCT()
struct FRemWidgetComponentTestMultiSoft : public FRemComponentBase
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<TSoftObjectPtr<UTextBlock>> Texts;
};
}

/**
 * Test host widget exposing the protected lifecycle entry points
 * (NativeTick / NativeDestruct) to automation specs.
 */
UCLASS()
class URemWidgetComponentTestWidget : public URemComponentBasedWidget
{
    GENERATED_BODY()

public:
    void PublicNativeTick(const FGeometry& Geometry, const float DeltaTime);

    void PublicNativeDestruct();
};

// --- inline definitions at file bottom ---
inline void URemWidgetComponentTestWidget::PublicNativeTick(const FGeometry& Geometry, const float DeltaTime)
{
    NativeTick(Geometry, DeltaTime);
}

inline void URemWidgetComponentTestWidget::PublicNativeDestruct()
{
    NativeDestruct();
}
