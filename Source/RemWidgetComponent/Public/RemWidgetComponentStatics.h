// Copyright RemRemRemRe. 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class UUserWidget;
struct FRemComponentContainer;

namespace Rem::WidgetComponent
{
/**
 * Resolves every TSoftObjectPtr<UWidget> field on every component against the
 * widget tree of the given widget, by the object name embedded in the soft path.
 *
 * The name is the decoupling contract: the same component can be reused on any
 * widget that contains a widget with the matching name. Linking is skipped at
 * design time.
 */
REMWIDGETCOMPONENT_API
void LinkComponentsToWidgetTree(const UUserWidget& UserWidget, FRemComponentContainer& Components);

}
