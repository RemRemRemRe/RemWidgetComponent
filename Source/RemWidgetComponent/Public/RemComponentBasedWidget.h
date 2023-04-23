// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RemWidgetComponentBase.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "RemComponentBasedWidget.generated.h"


/**
 * A widget who will have many components ^^
 */
UCLASS()
class REMWIDGETCOMPONENT_API URemComponentBasedWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Data Members

	UPROPERTY(EditAnywhere, Instanced, BlueprintGetter = GetComponents, Category = "Rem|Component")
	TArray<TObjectPtr<URemWidgetComponentBase>> Components;

#pragma endregion Data Members

#pragma region Members Accessors

public:
	UFUNCTION(BlueprintCallable, Category = "Rem|Component")
	TArray<URemWidgetComponentBase*> GetComponents() const;

	TArray<TObjectPtr<URemWidgetComponentBase>> GetComponentsObjectPtr() const;

	FArrayProperty* GetComponentsProperty() const;

#pragma endregion Members Accessors

public:
	virtual bool Initialize() override;

#if WITH_EDITOR

	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;

#endif

};
