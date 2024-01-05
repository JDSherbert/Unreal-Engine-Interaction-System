// ©2023 JDSherbert. All Rights Reserved.

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include <Runtime/Engine/Classes/Components/ActorComponent.h>
#include <Runtime/Engine/Classes/GameFramework/HUD.h>
#include <Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "InteractorComponent.generated.h"

/* ---------------------------- Forward Declarations ----------------------------- */

class AHUD;
class APlayerController;
class UInteractableComponent;
class UInputAction;
class UUserWidget;

/* ------------------------------ Class Definition ------------------------------- */

/**
* Interactor Component Class. Performs behaviors related to interactions.
* Receives input from EnhancedInputSystem Plugin and assigns performs interactions based on definitions in this class.
* Make sure to assign a UInputAction to define the bindings for this behavior!
* @since 19/01/2023
* @author JDSherbert
*/
UCLASS( ClassGroup = "Sherbert", Blueprintable, meta = (BlueprintSpawnableComponent))
class SHERBERT_API UInteractorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInteractorComponent(const FObjectInitializer& ObjectInitializer);

private:

	/** Interact Input Action - Assign bindings here. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sherbert|Component|Interaction|Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionInputAction;

	/* Max distance to use/focus on actors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sherbert|Component|Interaction", meta = (AllowPrivateAccess = "true"))
	float MaxInteractionRange;

	/* True only in first frame when focused on new usable actor. */
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Sherbert|Component|Interaction")
	bool bHasNewFocus;

	/* Actor derived from UsableActor currently in center-view. */
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Sherbert|Component|Interaction")
	UInteractableComponent* FocusedInteractableComponent;

	/** Turn on logs and lasers. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sherbert|Component|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bDebugMode;

	/* The associated player controller. */
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Sherbert|Component|Interaction")
	APlayerController* PlayerController;

	/* UI to display when an interaction can occur. */
	UPROPERTY(EditDefaultsOnly, Category = "Sherbert|Component|Interaction|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InteractionUITemplate;
	UUserWidget* InteractionUIInstance;

	/* UI to display when an interaction cannot occur. */
	UPROPERTY(EditDefaultsOnly, Category = "Sherbert|Component|Interaction|UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> NoInteractionUITemplate;
	UUserWidget* NoInteractionUIInstance;

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	/**
	* Initializer method.
	* @since 17/01/2023
	* @author JDSherbert
	*/
	void Init();

public:

	/**
	* Should be called only by an Interactor Component once during initiation.
	* Creates a UUserWidget to display cosmetically to the player when an interaction can occur.
	* @return UUserWidget* : The interaction widget that was created. Will also be assigned to InteractionUIInstance. Returns nullptr on fail.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Sherbert|Component|Interaction")
	UUserWidget* MakeInteractionUIWidget();

	/**
	* Should be called only by an Interactor Component once during initiation.
	* Creates a UUserWidget to display cosmetically to the player when an interaction cannot occur.
	* @return UUserWidget* : The non-interaction widget that was created. Will also be assigned to NoInteractionUIInstance. Returns nullptr on fail.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Sherbert|Component|Interaction")
	UUserWidget* MakeNoInteractionUIWidget();

	/**
	* Raycast method. Scans for an interactable component on a hit object and returns the currently looked at object.
	* Called every tick.
	* @return UInteractableComponent* : The Interactable Component that is being looked at.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	UInteractableComponent* RaycastForInteractable();

	/**
	* Setter method. Assigns the currently focused interactable component into the cache.
	* Invokes OnLookAt event.
	* @param NewInteractableComponent : Sets this as the currently focused interactable.
	* @return UInteractableComponent* : The Interactable Component that is currently focused.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	UInteractableComponent* AssignFocusedInteractable(UInteractableComponent* NewInteractableComponent);

	/**
	* Unsetter method. Unassigns the currently focused interactable component in the cache (sets to nullptr).
	* Invokes OnLookAway event.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	void UnassignFocusedInteractable();

	/**
	* Display UI method. Cosmetic. Based on input params, will show an interaction or no interaction widget, and hide them when not in use.
	* @param bActive : Whether to show or hide the UI Widget.
	* @param bCanInteract : If the interactable component is enabled, show the interact UI. Otherwise, show the no interaction UI.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Sherbert|Component|Interaction")
	void DisplayInteractionUIWidget(const bool bActive = false, const bool bCanInteract = true);

	/**
	* Interaction method. Sends a message to the interactable component on the focused object in a generic way by using events on that component. 
	* @param Instigator : Should always be "this", or "self". Lets the interactable know who or what is interacting with it.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	void Interact(UInteractorComponent* Instigator);

	/**
	* Comparison method. Returns true if the new interactable component is the same as the currently focused interactable in the cache.
	* Used to help prevent multiple calls to OnLookAt events on the same interactable.
	* @param NewInteractableComponent : The new interactable to compare.
	* @return bool : True if NewInteractableComponent matches the cached one.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sherbert|Component|Interaction")
	FORCEINLINE bool CompareInteractable(UInteractableComponent* NewInteractableComponent) { return NewInteractableComponent == FocusedInteractableComponent; }

	/**
	* Getter method. Returns the first Interactable Component from an actor if one exists.
	* @param Actor : The actor to try get the component from.
	* @return UInteractableComponent* : The InteractableComponent, if one is found. Otherwise, returns nullptr.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sherbert|Component|Interaction")
	InteractableComponent* GetInteractableComponent(const AActor* Actor);

	/**
	* Getter method. Returns the owner's HUD, if it has one.
	* @return AHUD* : The HUD, if one is found. Otherwise, returns nullptr.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sherbert|Component|Interaction")
	FORCEINLINE AHUD* GetInteractorHUD() const { return GetInteractorPlayerController()->GetHUD(); }

	/**
	* Getter method. Returns the owner's PlayerController, if it has one.
	* @return APlayerController* : The PlayerController, if one is found. Otherwise, returns nullptr.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sherbert|Component|Interaction")
	FORCEINLINE APlayerController* GetInteractorPlayerAgentController() const {return PlayerController; }

	/**
	* Getter method. Returns the Interaction UI Widget instance, if there is one.
	* @return UUserWidget* : The UI Widget, if one is found. Otherwise, returns nullptr.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintCosmetic, Category = "Sherbert|Component|Interaction")
	FORCEINLINE UUserWidget* GetInteractionUIInstance() const { return InteractionUIInstance; }

	/**
	* Getter method. Returns the No Interaction UI Widget instance, if there is one.
	* @return UUserWidget* : The UI Widget, if one is found. Otherwise, returns nullptr.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintCosmetic, Category = "BlueGhost|Component|Interaction")
	FORCEINLINE UUserWidget* GetNoInteractionUIInstance() const { return NoInteractionUIInstance; }

	/* ------------------------------ Events ------------------------------- */

	/**
	* Event: Triggers when an interactable component containing object is looked at.
	* @param @param Target : The interactable component that is being looked at.
	* @since 17/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Interaction")
	void Event_OnLookAt(UInteractableComponent* Target);

	/**
	* Event: Triggers when an interactable containing object is looked away from.
	* @param Target : The interactable component that is being looked away from.
	* @since 17/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Interaction")
	void Event_OnLookAway(UInteractableComponent* Target);

	/**
	* Event: Triggers when the interactable component containing object is interacted with.
	* @param Target : The interactable component that is being interacted with.
	* @since 17/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Interaction")
	void Event_OnInteraction(UInteractableComponent* Target);
};

/* ------------------------------------------------------------------------------- */
