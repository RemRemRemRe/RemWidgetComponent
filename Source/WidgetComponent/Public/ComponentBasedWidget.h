// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetComponentBase.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "ComponentBasedWidget.generated.h"


/** Wrapper struct for customization of components */
USTRUCT()
struct WIDGETCOMPONENT_API FWidgetComponentContainer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, Category = Component)
	TArray<TObjectPtr<UWidgetComponentBase>> Components;
};

/**
 * A widget who will have many components ^^
 */
UCLASS()
class WIDGETCOMPONENT_API UComponentBasedWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Data Members

private:
	UPROPERTY(EditAnywhere, Category = Component)
	FWidgetComponentContainer Components;

#pragma endregion Data Members

#pragma region Members Accessors

public:
	FORCEINLINE TArray<TObjectPtr<UWidgetComponentBase>> GetComponents() const;
	
#pragma endregion Members Accessors
	
public:
	virtual bool Initialize() override;
};
