// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponentBase.h"

#include "GameFrameworkStatics.h"
#include "WidgetComponentStats.h"
#include "Object/ObjectStatics.h"

void UWidgetComponentBase::Initialize()
{
	Super::Initialize();

	InitializeComponent();
	BeginPlay();
}

void UWidgetComponentBase::Destruct()
{
	EndPlay();
	UninitializeComponent();
	
	Super::Destruct();
}

bool UWidgetComponentBase::RequiresTick() const
{
	if (bImplementedRequiresTick)
	{
		return BP_RequiresTick();
	}
	
	return Super::RequiresTick();
}

void UWidgetComponentBase::Tick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::Tick(MyGeometry, InDeltaTime);
	
	if (bIsBlueprintObject)
	{
		ReceiveTick(InDeltaTime);
	}
}

UUserWidget* UWidgetComponentBase::GetUserWidget() const
{
	return Super::GetUserWidget();
}

void UWidgetComponentBase::InitializeComponent()
{
	bIsBlueprintObject = UGameFrameworkStatics::IsBlueprintObject(this);

	using namespace Common::ObjectStatics;
	bImplementedReceiveBeginPlay	= bIsBlueprintObject && IsImplementedInBlueprint(
		GetClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UWidgetComponentBase, ReceiveBeginPlay)));

	bImplementedReceiveEndPlay		= bIsBlueprintObject && IsImplementedInBlueprint(
		GetClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UWidgetComponentBase, ReceiveEndPlay)));

	bImplementedRequiresTick		= bIsBlueprintObject && IsImplementedInBlueprint(
		GetClass()->FindFunctionByName(GET_FUNCTION_NAME_CHECKED(UWidgetComponentBase, BP_RequiresTick)));
}

void UWidgetComponentBase::BeginPlay()
{
	if (bImplementedReceiveBeginPlay)
	{
		ReceiveBeginPlay();
	}
}

void UWidgetComponentBase::EndPlay()
{
	if (bImplementedReceiveEndPlay)
	{
		ReceiveEndPlay();
	}
}

void UWidgetComponentBase::UninitializeComponent()
{
}

TStatId UWidgetComponentBase::GetStatId() const
{
	return GET_STATID(STAT_WidgetComponent);
}
