// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "RemWidgetComponentBase.h"
#include "Blueprint/UserWidget.h"
#include "RemComponentBasedWidget.generated.h"


/**
 * A widget who will have many components ^^
 */
UCLASS()
class REMWIDGETCOMPONENT_API URemComponentBasedWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Data Members

	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetComponents, Category = "Component")
	TArray<TObjectPtr<URemWidgetComponentBase>> Components;

#pragma endregion Data Members

public:
	virtual bool Initialize() override;

#if WITH_EDITOR

	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;

#endif
	
#pragma region Members Accessors

public:
	UFUNCTION(BlueprintCallable, Category = "Rem|Component")
	TArray<URemWidgetComponentBase*> GetComponents() const;

	TArray<TObjectPtr<URemWidgetComponentBase>> GetComponentsObjectPtr() const;

	FArrayProperty* GetComponentsProperty() const;

#pragma endregion Members Accessors
};
