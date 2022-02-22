// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tickable.h"
#include "UObject/Object.h"
#include "WidgetComponentBase.generated.h"

/**
 * Base class for a component of an UComponentBasedWidget ^^
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class WIDGETCOMPONENT_API UWidgetComponentBase : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Initializes the component
	 */
	virtual void InitializeComponent();
	
	/**
	 * @brief Begins Play for the component
	 * Called when the owning widget begins play or when the component is created if the Actor has already begun play.
	 */
	virtual void BeginPlay();

	/** 
	 * Blueprint implementable event for when the component is beginning play, called before its owning actor's BeginPlay
	 * or when the component is dynamically created if the Actor has already BegunPlay. 
	 */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Begin Play"))
	void ReceiveBeginPlay();

	/**
	 * @brief Ends gameplay for this component.
	 * Called from AActor::EndPlay only if bHasBegunPlay is true
	 */
	virtual void EndPlay();

	/** Blueprint implementable event for when the component ends play, generally via destruction or its Actor's EndPlay. */
	UFUNCTION(BlueprintImplementableEvent, meta=(Keywords = "delete", DisplayName = "End Play"))
	void ReceiveEndPlay();
	
	/**
	 * Handle this component being Uninitialized.
	 */
	virtual void UninitializeComponent();

	virtual void Tick( float DeltaTime ) override;
	virtual TStatId GetStatId() const override;

	/** Event called every frame if tick is enabled */
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "Tick"))
	void ReceiveTick(float DeltaSeconds);
};
