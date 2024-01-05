![image](https://github.com/JDSherbert/Unreal-Engine-Interaction-System/assets/43964243/1f3c86cb-4b3e-4d19-a609-3716d9c0d247)

# Unreal Engine Interaction System

<!-- Header Start -->
<a href = "https://docs.unrealengine.com/5.3/en-US/"> <img height="40" img width="40" src="https://cdn.simpleicons.org/unrealengine/white"> </a> 
<a href = "https://learn.microsoft.com/en-us/cpp/cpp-language"> <img height="40" img width="40" src="https://cdn.simpleicons.org/c++"> </a>
<img align="right" alt="Stars Badge" src="https://img.shields.io/github/stars/jdsherbert/Unreal-Engine-Interaction-System?label=%E2%AD%90"/>
<img align="right" alt="Forks Badge" src="https://img.shields.io/github/forks/jdsherbert/Unreal-Engine-Interaction-System?label=%F0%9F%8D%B4"/>
<img align="right" alt="Watchers Badge" src="https://img.shields.io/github/watchers/jdsherbert/Unreal-Engine-Interaction-System?label=%F0%9F%91%81%EF%B8%8F"/>
<img align="right" alt="Issues Badge" src="https://img.shields.io/github/issues/jdsherbert/Unreal-Engine-Interaction-System?label=%E2%9A%A0%EF%B8%8F"/>
<img align="right" src="https://hits.seeyoufarm.com/api/count/incr/badge.svg?url=https%3A%2F%2Fgithub.com%2FJDSherbert%2FUnreal-Engine-Interaction-System%2Fhit-counter%2FREADME&count_bg=%2379C83D&title_bg=%23555555&labelColor=0E1128&title=🔍&style=for-the-badge">
<!-- Header End --> 

-----------------------------------------------------------------------

<a href="https://docs.unrealengine.com/5.3/en-US/"> 
  <img align="left" alt="Unreal Engine Template" src="https://img.shields.io/badge/Unreal%20Engine%20Component-black?style=for-the-badge&logo=unrealengine&logoColor=white&color=black&labelColor=black"> </a>
  
<a href="https://choosealicense.com/licenses/mit/"> 
  <img align="right" alt="License" src="https://img.shields.io/badge/License%20:%20MIT-black?style=for-the-badge&logo=mit&logoColor=white&color=black&labelColor=black"> </a>
  
<br></br>

-----------------------------------------------------------------------
## Overview
This interaction system provides a simple and modular solution for handling interactions between an interactor (InteractorComponent) and interactable actors (InteractableComponent) in Unreal Engine. The system facilitates communication between these components through a defined interface, allowing you to easily implement and customize interactions within your projects!

### Components
#### - Interactor
The Interactor component is responsible for initiating interactions with interactable objects. It is typically attached to the player character, or any other actor that needs interaction capabilities.

##### Usage:
- Attach to Character: Add the Interactor component to your player character or desired actor.
- Input Action: Configure an input action for interaction (e.g., "Interact"). Bind this action in your player controller or wherever input handling is managed.

#### Interactable
The Interactable component represents objects in the world that can be interacted with. It responds to interaction requests from Interactors.

##### Usage:
- Attach to Actors: Add the Interactable component to any actor you want to make interactable.
- Implement Interface: Make sure the actor implements the IInteractableInterface interface. This interface defines the methods needed for interaction.
- Interaction Interface (IInteractableInterface)
- The interface defines the communication contract between Interactors and Interactables. It includes methods like OnInteract that need to be implemented by Interactable actors.

Implementation Steps
Input Action:

In your project settings, define an input action for interaction (e.g., "Interact").
Bind this input action to the desired key or button in your player controller.
UUserWidgets:

Create UUserWidgets (UI) for any interactive feedback you want to provide to the player during interactions.
These widgets can be triggered by the Interactable actors during interaction events.
Attach Components:

Attach the Interactor component to your player character or actor that will perform interactions.
Attach the Interactable component to any actor you want to make interactable.
Implement Interaction:

Make sure the Interactable actors implement the IInteractableInterface.
Implement the OnInteract method in Interactable actors to define the behavior of the interaction.
Interaction Logic:

In your player controller or wherever input is handled, call the Interact method on the attached Interactor when the interaction input is triggered.

-----------------------------------------------------------------------

