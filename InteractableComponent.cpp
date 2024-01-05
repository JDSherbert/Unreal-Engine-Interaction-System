// ©2023 JDSherbert. All Rights Reserved.

#include "Project/Public/Components/InteractableComponent.h"

#include "Project/Public/Components/InteractorComponent.h"

/* ---------------------------- Method Definitions ------------------------------- */

UInteractableComponent::UInteractableComponent(const FObjectInitializer& ObjectInitializer)
	: bInteractable(true)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

/* ------------------------------------------------------------------------------- */

void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();
}

/* ------------------------------------------------------------------------------- */

void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/* ------------------------------------------------------------------------------- */

void UInteractableComponent::Interact(UInteractorComponent* Instigator)
{
	if (GetIsInteractable())
	{
		Event_OnInteraction(Instigator);
	}
	
	// You can add additional functionality here if you like. 
	// Be aware it will apply to all of these, so I'd recommend inheriting this class instead and doing it there.
	// OR you could define/call an implemented interface for custom handling per object.
}

/* ------------------------------------------------------------------------------- */

void UInteractableComponent::LookAt(UInteractorComponent* Instigator, const bool bActive)
{
	if (GetIsInteractable())
	{
		Instigator->DisplayInteractionUIWidget(bActive);
		(bActive) ? Event_OnLookAt(Instigator) : Event_OnLookAway(Instigator);
	}
}

/* ------------------------------------------------------------------------------- */

bool UInteractableComponent::SetPhysicsActive(bool bActive /*= false*/)
{
	if (AActor* Owner = GetOwner())
	{
		if (UActorComponent* ActorComponent = Owner->GetComponentByClass(UPrimitiveComponent::StaticClass()))
		{
			if (UPrimitiveComponent* PrimitiveComponent = CastChecked<UPrimitiveComponent>(ActorComponent))
			{
				PrimitiveComponent->SetSimulatePhysics(bActive);
				return PrimitiveComponent->IsSimulatingPhysics();
			}
		}
	}

	return false;
}

/* ------------------------------------------------------------------------------- */
