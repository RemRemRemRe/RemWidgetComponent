// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemWidgetComponentTestStructs.h"

#include "Components/RemComponentContainer.inl"
#include "Misc/AutomationTest.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"
#include "Layout/Geometry.h"

#if WITH_DEV_AUTOMATION_TESTS

TArray<int32> Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters::InitializeOrder;
TArray<int32> Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters::UninitializeOrder;
TArray<int32> Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters::TickOrder;
TArray<float> Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters::TickDeltas;
bool Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters::bLinkedBeforeInitialize = false;

void Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters::Reset()
{
    InitializeOrder.Reset();
    UninitializeOrder.Reset();
    TickOrder.Reset();
    TickDeltas.Reset();
    bLinkedBeforeInitialize = false;
}

void Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking::Initialize(const FContext& Context)
{
    FRemWidgetComponentTestCounters::InitializeOrder.Add(Context.ComponentIndex);
    FRemWidgetComponentTestCounters::bLinkedBeforeInitialize = Text.Get() != nullptr;
}

bool Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking::ShouldTick(const FContext&) const
{
    return true;
}

void Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking::Tick(const FContext& Context,
    const float DeltaSeconds)
{
    FRemWidgetComponentTestCounters::TickOrder.Add(Context.ComponentIndex);
    FRemWidgetComponentTestCounters::TickDeltas.Add(DeltaSeconds);
}

void Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking::Uninitialize(const FContext& Context)
{
    FRemWidgetComponentTestCounters::UninitializeOrder.Add(Context.ComponentIndex);
}

void Rem::WidgetComponent::Private::FRemWidgetComponentTestIdle::Initialize(const FContext& Context)
{
    FRemWidgetComponentTestCounters::InitializeOrder.Add(Context.ComponentIndex);
}

bool Rem::WidgetComponent::Private::FRemWidgetComponentTestIdle::ShouldTick(const FContext&) const
{
    return false;
}

void Rem::WidgetComponent::Private::FRemWidgetComponentTestIdle::Tick(const FContext& Context,
    const float DeltaSeconds)
{
    FRemWidgetComponentTestCounters::TickOrder.Add(Context.ComponentIndex);
    FRemWidgetComponentTestCounters::TickDeltas.Add(DeltaSeconds);
}

void Rem::WidgetComponent::Private::FRemWidgetComponentTestIdle::Uninitialize(const FContext& Context)
{
    FRemWidgetComponentTestCounters::UninitializeOrder.Add(Context.ComponentIndex);
}

using Rem::WidgetComponent::Private::FRemWidgetComponentTestCounters;
using Rem::WidgetComponent::Private::FRemWidgetComponentTestIdle;
using Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking;

DEFINE_SPEC(FRemWidgetComponentLifecycleTest, "Rem.WidgetComponent.Lifecycle",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemWidgetComponentLifecycleTest::Define()
{
    const auto SetupWidget = []() -> URemWidgetComponentTestWidget*
    {
        auto* Widget = NewObject<URemWidgetComponentTestWidget>();
        Widget->WidgetTree = NewObject<UWidgetTree>(Widget);

        auto* TextBlock = NewObject<UTextBlock>(Widget->WidgetTree, TEXT("HealthText"));
        Widget->WidgetTree->RootWidget = TextBlock;

        TArray<TInstancedStruct<FRemComponentBase>> Components;
        auto Ticking = TInstancedStruct<FRemComponentBase>::Make<FRemWidgetComponentTestTicking>();
        Ticking.GetMutable<FRemWidgetComponentTestTicking>().Text = FSoftObjectPath(
            TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText"));
        Ticking.GetMutable<FRemWidgetComponentTestTicking>().Value = 5;
        Components.Add(std::move(Ticking));
        Components.Add(TInstancedStruct<FRemComponentBase>::Make<FRemWidgetComponentTestIdle>());

        Widget->GetComponentContainer()->SetComponentsView(std::move(Components));
        return Widget;
    };

    Describe(TEXT("Initialize"), [this, SetupWidget]
    {
        It(TEXT("should link soft references before initializing components"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->Initialize();

            TestTrue(TEXT("soft ref linked before component Initialize"),
                FRemWidgetComponentTestCounters::bLinkedBeforeInitialize);
        });

        It(TEXT("should initialize components in container order with the correct context"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->Initialize();

            TestEqual(TEXT("initialize order"), FRemWidgetComponentTestCounters::InitializeOrder.Num(), 2);
            if (FRemWidgetComponentTestCounters::InitializeOrder.Num() == 2)
            {
                TestEqual(TEXT("first index"), FRemWidgetComponentTestCounters::InitializeOrder[0], 0);
                TestEqual(TEXT("second index"), FRemWidgetComponentTestCounters::InitializeOrder[1], 1);
            }
            TestTrue(TEXT("container initialized"), Widget->GetComponentContainer()->IsInitialized());
        });

        It(TEXT("should no-op on a second Initialize"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->Initialize();
            Widget->Initialize();

            TestEqual(TEXT("initialized once"), FRemWidgetComponentTestCounters::InitializeOrder.Num(), 2);
        });

        It(TEXT("should skip linking and Initialize at design time"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->SetDesignerFlags(EWidgetDesignFlags::Designing);
            Widget->Initialize();

            TestEqual(TEXT("no initialize dispatch"), FRemWidgetComponentTestCounters::InitializeOrder.Num(), 0);
            TestFalse(TEXT("not initialized"), Widget->GetComponentContainer()->IsInitialized());
        });
    });

    Describe(TEXT("NativeDestruct"), [this, SetupWidget]
    {
        It(TEXT("should uninitialize components"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->Initialize();
            Widget->PublicNativeDestruct();

            TestEqual(TEXT("uninitialize count"), FRemWidgetComponentTestCounters::UninitializeOrder.Num(), 2);
            if (FRemWidgetComponentTestCounters::UninitializeOrder.Num() == 2)
            {
                TestEqual(TEXT("first index"), FRemWidgetComponentTestCounters::UninitializeOrder[0], 0);
                TestEqual(TEXT("second index"), FRemWidgetComponentTestCounters::UninitializeOrder[1], 1);
            }
            TestFalse(TEXT("not initialized"), Widget->GetComponentContainer()->IsInitialized());
        });
    });

    Describe(TEXT("NativeTick"), [this, SetupWidget]
    {
        It(TEXT("should tick components only when ShouldTick returns true"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->Initialize();
            Widget->PublicNativeTick(FGeometry{}, 0.016f);

            // Index 0 (Ticking) ticks; index 1 (Idle) has ShouldTick == false.
            TestEqual(TEXT("tick order"), FRemWidgetComponentTestCounters::TickOrder,
                TArray<int32>({0}));
            TestEqual(TEXT("tick delta"), FRemWidgetComponentTestCounters::TickDeltas.Num(), 1);
            if (FRemWidgetComponentTestCounters::TickDeltas.Num() == 1)
            {
                TestEqual(TEXT("delta value"), FRemWidgetComponentTestCounters::TickDeltas[0], 0.016f);
            }
        });

        It(TEXT("should dispatch polymorphically through base virtuals"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->Initialize();
            Widget->PublicNativeTick(FGeometry{}, 0.5f);

            // Ticking (index 0) dispatched its own Tick override.
            TestEqual(TEXT("tick count"), FRemWidgetComponentTestCounters::TickOrder.Num(), 1);
            if (FRemWidgetComponentTestCounters::TickOrder.Num() == 1)
            {
                TestEqual(TEXT("ticking index"), FRemWidgetComponentTestCounters::TickOrder[0], 0);
                TestEqual(TEXT("delta"), FRemWidgetComponentTestCounters::TickDeltas[0], 0.5f);
            }
        });

        It(TEXT("should not tick before Initialize"), [this, SetupWidget]
        {
            FRemWidgetComponentTestCounters::Reset();

            auto* Widget = SetupWidget();
            Widget->PublicNativeTick(FGeometry{}, 0.016f);

            TestEqual(TEXT("no tick before initialize"), FRemWidgetComponentTestCounters::TickOrder.Num(), 0);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
