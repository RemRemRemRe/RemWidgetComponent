// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WidgetComponentBase.h"
#include "WidgetComponentAsExtension.generated.h"

/**
 * @brief Server as the bridge to hoop "component" into "UserWidgetExtension"
 *
 * It has two main purposes :
 * 
 * 1. call WidgetComponentStatics::LinkSoftObjectToRuntimeVariable on Initialize
 * 2. provide information about which member to detail customize
 *		@see FComponentBasedWidgetDetails::CustomizeDetails
 */
UCLASS()
class WIDGETCOMPONENT_API UWidgetComponentAsExtension : public UWidgetComponentBase
{
	GENERATED_BODY()

#pragma region Data Members

	UPROPERTY(EditAnywhere, Category = Component)
	TFieldPath<FArrayProperty> ComponentsFiledPath;

#pragma endregion Data Members
	
public:
	TFieldPath<FArrayProperty> GetComponentsFiledPath() const {	return ComponentsFiledPath; }
	
	void SetComponentsFiledPath(TFieldPath<FArrayProperty> InComponentsFiledPath);
	
	FArrayProperty* GetComponentsProperty() const;

#pragma region UserWidgetExtension Interface
	
	virtual void Initialize() override;
	virtual bool RequiresTick() const override;

#pragma endregion UserWidgetExtension Interface
};
