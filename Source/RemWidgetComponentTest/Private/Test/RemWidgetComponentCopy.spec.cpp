// Copyright RemRemRemRe. 2026. All Rights Reserved.

#include "Test/RemWidgetComponentTestStructs.h"

#include "Components/RemComponentContainer.inl"
#include "Misc/AutomationTest.h"
#include "Serialization/MemoryReader.h"
#include "Serialization/MemoryWriter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

#if WITH_DEV_AUTOMATION_TESTS

using Rem::WidgetComponent::Private::FRemWidgetComponentTestIdle;
using Rem::WidgetComponent::Private::FRemWidgetComponentTestTicking;

DEFINE_SPEC(FRemWidgetComponentCopyTest, "Rem.WidgetComponent.Copy",
    EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::ProductFilter);

void FRemWidgetComponentCopyTest::Define()
{
    const auto SetupWidget = []() -> URemComponentBasedWidget*
    {
        auto* Widget = NewObject<URemComponentBasedWidget>();
        Widget->WidgetTree = NewObject<UWidgetTree>(Widget);
        auto* TextBlock = NewObject<UTextBlock>(Widget->WidgetTree, TEXT("HealthText"));
        Widget->WidgetTree->RootWidget = TextBlock;

        TArray<TInstancedStruct<FRemComponentBase>> Components;
        auto Component = TInstancedStruct<FRemComponentBase>::Make<FRemWidgetComponentTestTicking>();
        Component.GetMutable<FRemWidgetComponentTestTicking>().Value = 7;
        Component.GetMutable<FRemWidgetComponentTestTicking>().Text = FSoftObjectPath(
            TEXT("/Game/Fake/FakeWidget.FakeWidget:WidgetTree.HealthText"));
        Components.Add(std::move(Component));
        Widget->GetComponentContainer()->SetComponentsView(std::move(Components));
        return Widget;
    };

    Describe(TEXT("Widget duplication"), [this, SetupWidget]
    {
        It(TEXT("should preserve edit-time config across widget duplication"), [this, SetupWidget]
        {
            auto* Widget = SetupWidget();
            Widget->Initialize();

            auto* Duplicated = DuplicateObject(Widget, GetTransientPackage());

            const auto View = Duplicated->GetComponentContainer()->FindComponent<FRemWidgetComponentTestTicking>();
            TestTrue(TEXT("component found"), View.IsValid());
            TestEqual(TEXT("value preserved"), View->Value, 7);

            // Duplication copies the whole widget tree (with a fresh name) and fixes the
            // soft reference up to the copy's own widget: only the name contract (the
            // last sub-path segment) must survive.
            const FUtf8StringView SubPath = View->Text.ToSoftObjectPath().GetSubPathUtf8String();
            TestTrue(TEXT("name contract preserved"), SubPath.EndsWith(UTF8TEXT("HealthText")));
        });
    });

    Describe(TEXT("Serialization"), [this, SetupWidget]
    {
        It(TEXT("should roundtrip component config through a memory archive"), [this]
        {
            // NOTE: a plain FMemoryArchive cannot serialize UObject references
            // (FInstancedStruct stores its UScriptStruct as an object reference).
            // FObjectAndNameAsStringProxyArchive serializes them as strings, matching
            // the in-memory roundtrip pattern.
            TInstancedStruct<FRemComponentBase> Source =
                TInstancedStruct<FRemComponentBase>::Make<FRemWidgetComponentTestIdle>();
            Source.GetMutable<FRemWidgetComponentTestIdle>().Name = TEXT("Health");

            TArray<uint8> Bytes;
            {
                FMemoryWriter Writer(Bytes);
                FObjectAndNameAsStringProxyArchive Proxy(Writer, /*bLoadIfFindFails=*/false);
                Source.Serialize(Proxy);
            }

            TInstancedStruct<FRemComponentBase> Restored;
            {
                FMemoryReader Reader(Bytes);
                FObjectAndNameAsStringProxyArchive Proxy(Reader, /*bLoadIfFindFails=*/false);
                Restored.Serialize(Proxy);
            }

            TestTrue(TEXT("restored type"), Restored.GetScriptStruct() == FRemWidgetComponentTestIdle::StaticStruct());
            const auto* View = Restored.GetPtr<FRemWidgetComponentTestIdle>();
            TestNotNull(TEXT("component view"), View);
            TestEqual(TEXT("name roundtrip"), View->Name, FString(TEXT("Health")));
        });
    });
}

#endif // WITH_DEV_AUTOMATION_TESTS
