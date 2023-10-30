// Copyright RemRemRemRe, All Rights Reserved.

#pragma once

#include "Extensions/UserWidgetExtension.h"
#include "RemWidgetComponentBase.generated.h"

/**
 * Base class for a component of an URemComponentBasedWidget ^^
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class REMWIDGETCOMPONENT_API URemWidgetComponentBase : public UUserWidgetExtension
{
	GENERATED_BODY()

#pragma region Data Members

	uint8 bIsBlueprintObject			: 1;
	uint8 bImplementedReceiveBeginPlay	: 1;
	uint8 bImplementedReceiveEndPlay	: 1;
	uint8 bImplementedRequiresTick		: 1;

#pragma endregion Data Members
	
public:

#pragma region Object

	virtual void PreDuplicate(FObjectDuplicationParameters& DupParams) override;
	
	virtual UWorld* GetWorld() const override;

#pragma endregion Object

#pragma region UserWidgetExtension Interface
	
	virtual void Initialize() override;
	virtual void Destruct() override;
	virtual bool RequiresTick() const override;
	virtual void Tick(const FGeometry& MyGeometry, float InDeltaTime) override;

#pragma endregion UserWidgetExtension Interface
	
	/**
	 * @brief Initializes the component
	 */
	virtual void InitializeComponent();
	
	/**
	 * @brief Begins Play for the component
	 * Called when the owning widget initialize or when the component is created if the widget has initialized.
	 */
	virtual void BeginPlay();

	/** 
	 * Blueprint implementable event for when the component is beginning play
	 * Called when the owning widget initialize or when the component is created if the widget has initialized.
	 */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Begin Play")
	void ReceiveBeginPlay();

	/**
	 * @brief Ends gameplay for this component.
	 * Called from UWidgetComponentBase::Destruct
	 */
	virtual void EndPlay();

	/** Blueprint implementable event for when the component ends play */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "End Play")
	void ReceiveEndPlay();
	
	/**
	 * Handle this component being Uninitialized.
	 */
	virtual void UninitializeComponent();

	virtual TStatId GetStatId() const;

	/** Blueprint implementable event decide whether the component requires tick. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "RequiresTick")
	bool BP_RequiresTick() const;
	
	/** Event called every frame if tick is enabled */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "Tick")
	void ReceiveTick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, DisplayName = "GetUserWidget", meta = (DeterminesOutputType = "Class"))
	UUserWidget* BP_GetUserWidget(TSubclassOf<UUserWidget> Class) const;
};
