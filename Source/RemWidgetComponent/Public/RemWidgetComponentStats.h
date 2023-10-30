// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Stats/Stats.h"

//// Groups
DECLARE_STATS_GROUP(TEXT("WidgetComponent"), STATGROUP_WidgetComponent, STATCAT_Advanced);

//// Stats
DECLARE_CYCLE_STAT_EXTERN(TEXT("WidgetComponent Tick"), STAT_WidgetComponent, STATGROUP_WidgetComponent, REMWIDGETCOMPONENT_API);