// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

private:
	UPROPERTY(EditAnywhere, Category = Components, Instanced)
	TArray<TObjectPtr<UWidgetComponentBase>> Components;

#pragma endregion Data Members

#pragma region Members Accessors

public:
	FORCEINLINE TArray<TObjectPtr<UWidgetComponentBase>> GetComponents() const;
	
#pragma endregion Members Accessors
	
public:
	virtual bool Initialize() override;
};
