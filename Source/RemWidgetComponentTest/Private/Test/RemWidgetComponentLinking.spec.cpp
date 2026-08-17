// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemWidgetComponentTestStructs.h"

#include "Components/RemComponentContainer.inl"
#include "Misc/AutomationTest.h"
#include "RemWidgetComponentStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Components/Widget.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::WidgetComponent::Private::FRemWidgetComponentTestMultiSoft;
using Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking;

DEFINE_SPEC(FRemWidgetComponentLinkingTest, "Rem.WidgetComponent.Linking",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemWidgetComponentLinkingTest::Define()
{
    const auto MakeTree = [](UUserWidget& Widget, const TCHAR* WidgetName) -> UTextBlock*
    {
        Widget.WidgetTree = NewObject<UWidgetTree>(&Widget);
        auto* TextBlock = NewObject<UTextBlock>(Widget.WidgetTree, FName(WidgetName));
        Widget.WidgetTree->RootWidget = TextBlock;
        return TextBlock;
    };

    const auto MakeComponent = [](const TCHAR* SoftPath) -> TInstancedStruct<FRemComponentBase>
    {
        auto Component = TInstancedStruct<FRemComponentBase>::Make<FRemWidgetComponentTestTicking>();
        Component.GetMutable<FRemWidgetComponentTestTicking>().Text = FSoftObjectPath(SoftPath);
        return Component;
    };

    Describe(TEXT("LinkComponentsToWidgetTree"), [this, MakeTree, MakeComponent]
    {
        It(TEXT("should link soft widget references to runtime widgets by name"), [this, MakeTree, MakeComponent]
        {
            auto* Widget = NewObject<URemWidgetComponentTestWidget>();
            auto* TextBlock = MakeTree(*Widget, TEXT("HealthText"));

            TArray<TInstancedStruct<FRemComponentBase>> Components;
            Components.Add(MakeComponent(TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText")));
            Widget->GetComponentContainer()->SetComponentsView(std::move(Components));

            Rem::WidgetComponent::LinkComponentsToWidgetTree(*Widget, *Widget->GetComponentContainer());

            const auto View = Widget->GetComponentContainer()->FindComponent<FRemWidgetComponentTestTicking>();
            TestTrue(TEXT("component found"), View.IsValid());
            TestEqual(TEXT("linked to runtime widget"), View->Text.Get(), TextBlock);
        });

        It(TEXT("should re-link and overwrite stale references"), [this, MakeTree, MakeComponent]
        {
            auto* FirstWidget = NewObject<URemWidgetComponentTestWidget>();
            auto* FirstTextBlock = MakeTree(*FirstWidget, TEXT("HealthText"));

            TArray<TInstancedStruct<FRemComponentBase>> FirstComponents;
            FirstComponents.Add(MakeComponent(TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText")));
            FirstWidget->GetComponentContainer()->SetComponentsView(std::move(FirstComponents));
            Rem::WidgetComponent::LinkComponentsToWidgetTree(*FirstWidget, *FirstWidget->GetComponentContainer());

            // Simulate a stale copy: the component now points at the FIRST widget's tree.
            const auto FirstView = FirstWidget->GetComponentContainer()->FindComponent<FRemWidgetComponentTestTicking>();
            TestEqual(TEXT("stale link to first widget"), FirstView->Text.Get(), FirstTextBlock);

            // Re-link against a second widget: the stale weak pointer must be overwritten.
            auto* SecondWidget = NewObject<URemWidgetComponentTestWidget>();
            auto* SecondTextBlock = MakeTree(*SecondWidget, TEXT("HealthText"));

            TArray<TInstancedStruct<FRemComponentBase>> SecondComponents;
            SecondComponents.Add(MakeComponent(TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText")));
            SecondWidget->GetComponentContainer()->SetComponentsView(std::move(SecondComponents));
            Rem::WidgetComponent::LinkComponentsToWidgetTree(*SecondWidget, *SecondWidget->GetComponentContainer());

            const auto SecondView = SecondWidget->GetComponentContainer()->FindComponent<FRemWidgetComponentTestTicking>();
            TestEqual(TEXT("re-linked to second widget"), SecondView->Text.Get(), SecondTextBlock);
        });

        It(TEXT("should skip linking at design time"), [this, MakeTree, MakeComponent]
        {
            auto* Widget = NewObject<URemWidgetComponentTestWidget>();
            (void)MakeTree(*Widget, TEXT("HealthText"));

            TArray<TInstancedStruct<FRemComponentBase>> Components;
            Components.Add(MakeComponent(TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText")));
            Widget->GetComponentContainer()->SetComponentsView(std::move(Components));

            Widget->SetDesignerFlags(EWidgetDesignFlags::Designing);
            Rem::WidgetComponent::LinkComponentsToWidgetTree(*Widget, *Widget->GetComponentContainer());

            const auto View = Widget->GetComponentContainer()->FindComponent<FRemWidgetComponentTestTicking>();
            TestTrue(TEXT("component found"), View.IsValid());
            TestTrue(TEXT("not linked at design time"), View->Text.Get() == nullptr);
        });

        It(TEXT("should link soft refs in container-typed fields via recursive property iteration"), [this, MakeComponent]
        {
            auto* Widget = NewObject<URemWidgetComponentTestWidget>();
            Widget->WidgetTree = NewObject<UWidgetTree>(Widget);
            auto* Canvas = NewObject<UCanvasPanel>(Widget->WidgetTree, TEXT("RootCanvas"));
            auto* HealthText = NewObject<UTextBlock>(Widget->WidgetTree, TEXT("HealthText"));
            auto* ManaText = NewObject<UTextBlock>(Widget->WidgetTree, TEXT("ManaText"));
            Canvas->AddChild(HealthText);
            Canvas->AddChild(ManaText);
            Widget->WidgetTree->RootWidget = Canvas;

            TArray<TInstancedStruct<FRemComponentBase>> Components;
            auto Component = TInstancedStruct<FRemComponentBase>::Make<FRemWidgetComponentTestMultiSoft>();
            auto& MultiSoft = Component.GetMutable<FRemWidgetComponentTestMultiSoft>();
            MultiSoft.Texts.Emplace(FSoftObjectPath(TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText")));
            MultiSoft.Texts.Emplace(FSoftObjectPath(TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.ManaText")));
            Components.Add(std::move(Component));
            Widget->GetComponentContainer()->SetComponentsView(std::move(Components));

            Rem::WidgetComponent::LinkComponentsToWidgetTree(*Widget, *Widget->GetComponentContainer());

            const auto View = Widget->GetComponentContainer()->FindComponent<FRemWidgetComponentTestMultiSoft>();
            TestTrue(TEXT("component found"), View.IsValid());
            TestEqual(TEXT("first linked"), View->Texts[0].Get(), HealthText);
            TestEqual(TEXT("second linked"), View->Texts[1].Get(), ManaText);
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
