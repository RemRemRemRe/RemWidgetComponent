// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetComponentBase.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "ComponentBasedWidget.generated.h"


/**
 * A widget who will have many components ^^
 */
UCLASS()
class WIDGETCOMPONENT_API UComponentBasedWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Data Members

	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetComponents, Category = Component)
	TArray<TObjectPtr<UWidgetComponentBase>> Components;

#pragma endregion Data Members

#pragma region Members Accessors

public:
	UFUNCTION(BlueprintCallable, Category = Component)
	TArray<UWidgetComponentBase*> GetComponents() const;

	TArray<TObjectPtr<UWidgetComponentBase>> GetComponentsObjectPtr() const;

	FArrayProperty* GetComponentsProperty() const;

#pragma endregion Members Accessors

public:
	virtual bool Initialize() override;

#if WITH_EDITOR

	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;

#endif

};
