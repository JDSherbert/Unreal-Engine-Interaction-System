// ©2023 JDSherbert. All Rights Reserved.

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include <Runtime/Engine/Classes/Components/ActorComponent.h>

#include "Project/Public/Interfaces/InteractionInterface.h"

#include "InteractableComponent.generated.h"

/* ---------------------------- Forward Declarations ----------------------------- */

class UInteractorComponent;

/* ------------------------------ Class Definition ------------------------------- */

/**
* Interactable Component Class. Receives calls from an Interactor Component.
* Define custom behaviour in the "Interact" method and the event calls in blueprint.
* Make sure to assign a UInputAction to the Interactor Component to define the bindings for this behavior!
* @since 19/01/2023
* @author JDSherbert
*/
UCLASS(ClassGroup = "Sherbert", Blueprintable, meta = (BlueprintSpawnableComponent))
class SHERBERT_API UBlueGhost_InteractableComponent : public UActorComponent, public IInteractionInterface
{
	GENERATED_BODY()

public:	

	UInteractableComponent(const FObjectInitializer& ObjectInitializer);

private:

	/** True = Interactable, False = not interactable! */
	UPROPERTY(EditDefaultsOnly, Category = "Sherbert|Component|Interaction", meta = (AllowPrivateAccess = "true"))
	bool bInteractable;

protected:

	virtual void BeginPlay() override;

public:	

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* Interaction method. Should be called only by an Interactor Component when the input action is pressed, once.
	* @param Instigator : The interactor component that is interacting with this object. 
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	virtual void Interact(UInteractorComponent* Instigator) override;

	/**
	* LookAt method. Should be called only by an Interactor Component when the raycast hits this interactable, every tick.
	* @param Instigator : The Interactor component that is looking at this object.
	* @param bActive : Sets if this Interactable is being looked at (and thus remains in the interactor's cache) or is being looked away from.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	void LookAt(UInteractorComponent* Instigator, const bool bActive = false);

	/**
	* Setter method. Should be called if the physics are being handled in a custom way, such as for picking up the attached actor.
	* @param bActive : Sets if this Interactable is to be simulated by the physics engine.
	* @return bool : True if simulating physics.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, Category = "Sherbert|Component|Interaction")
	bool SetPhysicsActive(bool bActive = false);

	/**
	* Getter method. Returns true if this can be interacted with.
	* @return bool bInteractable : True if this is an interactable component that can be interacted with.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sherbert|Component|Interaction")
	FORCEINLINE bool GetIsInteractable() const { return bInteractable; }

	/**
	* Setter method. Sets if this component can be interacted with by an interactor. 
	* Useful for turning on/off interaction functionality such as for a cutscene.
	* @param bIsInteractable : Sets if this Interactable Component is interactable or not.
	* @return bool bInteractable : True if this is an interactable component that can be interacted with.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Sherbert|Component|Interaction")
	FORCEINLINE bool SetIsInteractable(const bool bIsInteractable = true) { bInteractable = bIsInteractable; return bInteractable; }

	/* ------------------------------ Events ------------------------------- */

	/**
	* Event: Triggers when the object is looked at.
	* @param Instigator : The Interactor component that is looking at this object. 
	* @since 17/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Interaction")
	void Event_OnLookAt(UInteractorComponent* Instigator);

	/**
	* Event: Triggers when the object is looked away from.
	* @param Instigator : The Interactor component that is looking away from this object.
	* @since 17/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Interaction")
	void Event_OnLookAway(UInteractorComponent* Instigator);

	/**
	* Event: Triggers when the object is interacted with.
	* @param Instigator : The interactor component that is interacting with this object. 
	* @since 17/01/2023
	* @author JDSherbert
	*/
	UFUNCTION(BlueprintImplementableEvent, Category = "Sherbert|Component|Interaction")
	void Event_OnInteraction(UInteractorComponent* Instigator);

};

/* ------------------------------------------------------------------------------- */
