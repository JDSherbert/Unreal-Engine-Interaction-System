// ©2023 JDSherbert. All Rights Reserved.

#pragma once

#include <Runtime/Core/Public/CoreMinimal.h>

#include <Runtime/CoreUObject/Public/UObject/Interface.h>

#include "InteractionInterface.generated.h"

/* ---------------------------- Forward Declarations ----------------------------- */

class UInteractableComponent;
class UInteractorComponent;

/* ------------------------------ Class Definition ------------------------------- */

/**
* Interactable Interface Class. Receives calls from an Interactor Component.
* Define custom behaviour in the inherited interface's "Interact" method.
* @since 20/01/2023
* @author JDSherbert
*/
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/* ---------------------------- Interface Definition ----------------------------- */

/**
* Interactable Component Interface. Receives calls from an Interactor Component.
* Define custom behaviour in the inherited interface's "Interact" method.
* @since 20/01/2023
* @author JDSherbert
*/
class SHERBERT_API IInteractionInterface
{
	GENERATED_BODY()

public:

	/**
	* Interaction method. Should be called only by an Interactor Component when the input action is pressed, once.
	* @param Instigator : The interactor component that is interacting with this object.
	* @since 27/01/2023
	* @author JDSherbert
	*/
	virtual void Interact(UInteractorComponent* Instigator) = 0;
};

/* ------------------------------------------------------------------------------- */
