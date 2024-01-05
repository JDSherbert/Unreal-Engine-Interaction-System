// ©2023 JDSherbert. All Rights Reserved.

#include "Project/Public/Components/InteractorComponent.h"

#include "Project/Public/Components/InteractableComponent.h"

#include <Runtime/Engine/Classes/Components/InputComponent.h>
#include <Runtime/Engine/Classes/GameFramework/Character.h>
#include <Runtime/Engine/Classes/GameFramework/HUD.h>
#include <Runtime/UMG/Public/Blueprint/UserWidget.h>

#include "EnhancedInput/Public/EnhancedInputComponent.h"
#include "EnhancedInput/Public/EnhancedInputSubsystems.h"

/* --------------------------------- Namespace ----------------------------------- */

namespace InteractorComponentDefs
{
	const TCHAR* DefaultTraceLineName = TEXT("Interaction Trace");
	const ECollisionChannel DefaultInteractionCollisionChannel = ECollisionChannel::ECC_Visibility;
	constexpr float DefaultMaxInteractionRange = 200.0f;
}

/* ---------------------------- Method Definitions ------------------------------- */

UInteractorComponent::UInteractorComponent(const FObjectInitializer& ObjectInitializer)
	: InteractInputAction(nullptr)
	, MaxInteractionRange(InteractorComponentDefs::DefaultMaxInteractionRange)
	, bHasNewFocus(false)
	, FocusedInteractableComponent(nullptr)
	, bDebugMode(false)
	, PlayerController(nullptr)
	, InteractionUITemplate(nullptr)
	, InteractionUIInstance(nullptr)
	, NoInteractionUITemplate(nullptr)
	, NoInteractionUIInstance(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

/* ------------------------------------------------------------------------------- */

void UInteractorComponent::BeginPlay()
{
	Super::BeginPlay();

	Init();
}

/* ------------------------------------------------------------------------------- */

void UInteractorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RaycastForInteractable();
}

/* ------------------------------------------------------------------------------- */

void UInteractorComponent::Init()
{
	if (const AActor* Owner = GetOwner())
	{
		if (const ACharacter* Character = CastChecked<ACharacter>(Owner))
		{
			if (UEnhancedInputComponent* TempInput = Character->EnhancedInputComponent)
			{
                		if(InteractionInputAction != nullptr)
                		{  
					TempInput->BindAction(InteractionInputAction, ETriggerEvent::Started, this, &UInteractorComponent::Interact, this);
                		}
                		else
                		{
                    			UE_LOG(LogTemp, Error, TEXT("No Input Action defined for Interactions. Add one to the Blueprint that is using this component, or assign one with constructor helpers."));
                		}
			}
			
			PlayerController = Character->GetPlayerController();
		}
	}

	InteractionUIInstance = MakeUIWidget(InteractionUITemplate, InteractionUIInstance,, "Interaction UI");
	NoInteractionUIInstance = MakeUIWidget(NoInteractionUITemplate, NoInteractionUIInstance, "NoInteraction UI"););
}

/* ------------------------------------------------------------------------------- */

UUserWidget* UInteractorComponent::MakeUIWidget(TSubclassOf<UUserWidget> Template, UUserWidget* Instance, FName UIName)
{
	// Create UI instance widget and apply to PlayerController
	if (Instance == nullptr && Template != nullptr) 
	{
		Instance = CreateWidget<UUserWidget>
		(
			GetInteractorPlayerController(),
			Template,
			UIName
		);
	}

	// Shove it into the viewport and collapse it to make it invisible
	if (Instance != nullptr)
	{
		Instance->AddToViewport(0);
		Instance->SetVisibility(ESlateVisibility::Collapsed);
	}

	return Instance;
}

/* ------------------------------------------------------------------------------- */

UInteractableComponent* UInteractorComponent::RaycastForInteractable()
{
	if (const AActor* Owner = this->GetOwner())
	{
		if (const ACharacter* Character = CastChecked<ACharacter>(Owner))
		{
			if (const AController* Controller = Character->Controller)
			{
				FVector camLoc;
				FRotator camRot;
				Controller->GetPlayerViewPoint(camLoc, camRot);
				const FVector TraceStart = camLoc;
				const FVector Direction = camRot.Vector();
				const FVector TraceEnd = TraceStart + (Direction * MaxInteractionRange);

				FCollisionQueryParams TraceParams(InteractorComponentDefs::DefaultTraceLineName, true, Owner);
				TraceParams.AddIgnoredActor(Owner);
				TraceParams.bDebugQuery = true;
				TraceParams.bReturnPhysicalMaterial = false;
				TraceParams.bTraceComplex = true;

				if (const UWorld* World = GetWorld())
				{
					FHitResult OutHit(ForceInit);
					
					GetWorld()->LineTraceSingleByChannel
					(
						OutHit,
						TraceStart, 
						TraceEnd, 
						InteractorComponentDefs::DefaultInteractionCollisionChannel, 
						TraceParams
					);
					
					if (OutHit.bBlockingHit)
					{
						if (const AActor* Actor = OutHit.GetActor())
						{
							if (bDebugMode) DebugHit(Actor, TraceStart, TraceEnd);
							if (UInteractableComponent* InteractableComponent = GetInteractableComponent(Actor))
							{
								return AssignFocusedInteractable(InteractableComponent);
							}
						}
					}
				}

				UnassignFocusedInteractable();
			}
		}
	}
	
	return nullptr;
}

/* ------------------------------------------------------------------------------- */

UInteractableComponent* UInteractorComponent::AssignFocusedInteractable(UInteractableComponent* NewInteractableComponent)
{
	if (!CompareInteractable(NewInteractableComponent))
	{
		FocusedInteractableComponent = NewInteractableComponent;
		FocusedInteractableComponent->LookAt(this, true);
		Event_OnLookAt(FocusedInteractableComponent);
		if (FocusedInteractableComponent)
		{
			DisplayInteractionUIWidget(true, FocusedInteractableComponent->GetIsInteractable());
		}

		return FocusedInteractableComponent;
	}

	return nullptr;
}

/* ------------------------------------------------------------------------------- */

void UInteractorComponent::UnassignFocusedInteractable()
{
	if (FocusedInteractableComponent != nullptr)
	{
		FocusedInteractableComponent->LookAt(this, false);
		Event_OnLookAway(FocusedInteractableComponent);
		if (FocusedInteractableComponent) 
		{
			DisplayInteractionUIWidget(false, false);
		}
        
		FocusedInteractableComponent = nullptr;
	}
}

/* ------------------------------------------------------------------------------- */

void UInteractorComponent::DisplayInteractionUIWidget(const bool bActive /*= false*/, const bool bCanInteract /*= true*/)
{
	if (InteractionUIInstance != nullptr && NoInteractionUIInstance != nullptr)
	{
		if (bActive)
		{
			// Show Interaction UI, Hide No Interaction UI and vice versa for this interactable based on its interactability.
			InteractionUIInstance->SetVisibility  ( bCanInteract ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
			NoInteractionUIInstance->SetVisibility(!bCanInteract ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
		}
		else
		{
			// Hide the UI, as the component is not active.
			InteractionUIInstance->SetVisibility(ESlateVisibility::Collapsed);
			NoInteractionUIInstance->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

/* ------------------------------------------------------------------------------- */

void UInteractorComponent::Interact(UInteractorComponent* Instigator)
{
	if (FocusedInteractableComponent != nullptr)
	{
		FocusedInteractableComponent->Interact(Instigator);
		Event_OnInteraction(FocusedInteractableComponent);
	}
}

/* ------------------------------------------------------------------------------- */

UInteractableComponent* UInteractorComponent::GetInteractableComponent(const AActor* Actor)
{
	if (Actor != nullptr)
	{
		if (UActorComponent* InteractableComponentClass = Actor->GetComponentByClass(UInteractableComponent::StaticClass()))
		{
			if (UInteractableComponent* InteractableComponent = CastChecked<UInteractableComponent>(InteractableComponentClass))
			{
				return InteractableComponent;
			}
		}
	}

	return nullptr;
}

/* ------------------------------------------------------------------------------- */

