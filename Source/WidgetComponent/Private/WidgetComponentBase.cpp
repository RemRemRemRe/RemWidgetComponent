// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetComponentBase.h"

#include "GameFrameworkStatics.h"
#include "WidgetComponentStats.h"

void UWidgetComponentBase::InitializeComponent()
{
}

void UWidgetComponentBase::BeginPlay()
{
	if (UGameFrameworkStatics::IsBlueprintObject(this))
	{
		ReceiveBeginPlay();
	}
}

void UWidgetComponentBase::EndPlay()
{
	// If we're in the process of being garbage collected it is unsafe to call out to blueprints
	if (UGameFrameworkStatics::IsObjectValidForBlueprint(this) && UGameFrameworkStatics::IsBlueprintObject(this))
	{
		ReceiveEndPlay();
	}
}

void UWidgetComponentBase::UninitializeComponent()
{
}

void UWidgetComponentBase::Tick(const float DeltaTime)
{
	if (UGameFrameworkStatics::IsBlueprintObject(this))
	{
		ReceiveTick(DeltaTime);
	}
}

TStatId UWidgetComponentBase::GetStatId() const
{
	return GET_STATID(STAT_WidgetComponent);
}
