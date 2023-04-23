// Fill out your copyright notice in the Description page of Project Settings.


#include "RemWidgetComponentBase.h"

#include "RemMisc.h"
#include "RemWidgetComponentStats.h"
#include "Object/RemObjectStatics.h"
#include "RemWidgetComponentLog.h"

#if WITH_EDITOR

static FAutoConsoleVariable CVarWidgetComponentBaseFixCompileCrash(TEXT("WidgetComponentBase.FixCompileCrash"), true,
		TEXT("Editor crashes when hit compile and it says : Objects have the same fully qualified name but different paths."));

#endif

void URemWidgetComponentBase::PreDuplicate(FObjectDuplicationParameters& DupParams)
{
	Super::PreDuplicate(DupParams);

#if WITH_EDITOR

	if (!CVarWidgetComponentBaseFixCompileCrash->GetBool())
	{
		return;
	}
	
	// What code bellow fix is :
	// editor crashes when hit compile and it says :
	//	=== Critical error: ===
	//	Fatal error: [File:Unreal\UnrealEngine\Engine\Source\Runtime\CoreUObject\Private\UObject\UObjectGlobals.cpp] [Line: 3005] 
	//	Objects have the same fully qualified name but different paths.
	//	New Object: Component_C /Game/Developers/HUD/HUDTest.HUDTest:Widget_C_2.Component_C_0
	//	Existing Object: REINST_Component_C_35 /Game/Developers/HUD/HUDTest.HUDTest:Widget_C_2.Component_C_0*/

	// Steps to reproduce:
	// 1. prepare a widget with an instanced widget component added
	// 2. change and recompile the widget component's blueprint and save, notice the widget blueprint is also be marked dirty (* showed up)
	// 3. go to compile the widget blueprint
	// 4. crashed

	// If you encounter issues, you can try comment the code or set CVarTryCompileCrashFix to false,
	// then avoid doing the steps described above
	// hopefully, someday , me or someone, could solve this problem ;)
	{
		auto IsReInstance = [] (const FString& String)
		{
			const FString Pattern = TEXT("REINST_");
			return String.StartsWith(Pattern);
		};
		
		if (const bool bReInstanceOuterAndNoReInstanceClass =
			IsReInstance(GetOuter()->GetName())
			&& !IsReInstance(DupParams.DestClass->GetName()) && !IsReInstance(GetClass()->GetName());
			bReInstanceOuterAndNoReInstanceClass && DupParams.DestName == GetFName()
			&& !Rem::Common::IsClassDefaultObject(DupParams.DestOuter))
		{
			UObject* Object = this;
			do
			{
				Object->Rename(nullptr, GetTransientPackage(),
					REN_DoNotDirty | REN_DontCreateRedirectors | REN_ForceNoResetLoaders);

				UE_LOG(LogRemWidgetComponent, Log, TEXT("Renaming object new name : %s, old name : %s path name : %s"),
					*Object->GetName(), *DupParams.DestName.ToString(), *Object->GetPathName());

				Object->MarkAsGarbage();
				
				Object = StaticFindObjectFastInternal( /*Class=*/ nullptr, DupParams.DestOuter, DupParams.DestName, true);
			}
			while (Object);
		}
	}

#endif

}

void URemWidgetComponentBase::Initialize()
{
	Super::Initialize();

	InitializeComponent();
	BeginPlay();
}

void URemWidgetComponentBase::Destruct()
{
	EndPlay();
	UninitializeComponent();
	
	Super::Destruct();
}

bool URemWidgetComponentBase::RequiresTick() const
{
	if (bImplementedRequiresTick)
	{
		return BP_RequiresTick();
	}
	
	return Super::RequiresTick();
}

void URemWidgetComponentBase::Tick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
	
	if (bIsBlueprintObject)
	{
		ReceiveTick(InDeltaTime);
	}
}

void URemWidgetComponentBase::InitializeComponent()
{
	bIsBlueprintObject = URemObjectStatics::IsBlueprintObject(this);

	using namespace Rem::Common::Object;
	bImplementedReceiveBeginPlay	= bIsBlueprintObject && IsImplementedInBlueprint(
		GetClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(URemWidgetComponentBase, ReceiveBeginPlay)));

	bImplementedReceiveEndPlay		= bIsBlueprintObject && IsImplementedInBlueprint(
		GetClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(URemWidgetComponentBase, ReceiveEndPlay)));

	bImplementedRequiresTick		= bIsBlueprintObject && IsImplementedInBlueprint(
		GetClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(URemWidgetComponentBase, BP_RequiresTick)));
}

void URemWidgetComponentBase::BeginPlay()
{
	if (bImplementedReceiveBeginPlay)
	{
		ReceiveBeginPlay();
	}
}

void URemWidgetComponentBase::EndPlay()
{
	if (bImplementedReceiveEndPlay)
	{
		ReceiveEndPlay();
	}
}

void URemWidgetComponentBase::UninitializeComponent()
{
}

TStatId URemWidgetComponentBase::GetStatId() const
{
	return GET_STATID(STAT_WidgetComponent);
}
