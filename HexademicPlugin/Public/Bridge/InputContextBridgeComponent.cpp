#include "Core/InputContextBridgeComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"

UInputContextBridgeComponent::UInputContextBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInputContextBridgeComponent::BeginPlay()
{
    Super::BeginPlay();
    // Set initial input mode (e.g., to Combat or Traversal after loading)
    SetInputMode(EInputModeState::Traversal); // Example default
}

void UInputContextBridgeComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UnbindInputActions();
    Super::EndPlay(EndPlayReason);
}

void UInputContextBridgeComponent::SetInputMode(EInputModeState NewMode)
{
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("[InputBridge] No PlayerController found on owner. Cannot set input mode."));
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
    if (!Subsystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("[InputBridge] EnhancedInputLocalPlayerSubsystem not found. Cannot set input mode."));
        return;
    }

    // Clear existing mapping contexts
    Subsystem->ClearAllMappings();

    // Add new mapping context for the selected mode
    if (UInputMappingContext* Context = InputMappingContexts.FindRef(NewMode))
    {
        Subsystem->AddMappingContext(Context, 0); // High priority
        CurrentInputMode = NewMode;
        UE_LOG(LogTemp, Log, TEXT("[InputBridge] Switched input mode to: %s"), *UEnum::GetValueAsString(NewMode));

        // Rebind input actions for the new context
        BindInputActions();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[InputBridge] No InputMappingContext found for mode: %s"), *UEnum::GetValueAsString(NewMode));
    }
}

void UInputContextBridgeComponent::BindInputActions()
{
    UnbindInputActions(); // Clear previous bindings first

    APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
    if (!PlayerController) return;

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
    if (!EnhancedInputComponent) return;

    // Iterate through all mapped Input Actions and bind them
    for (const TPair<TObjectPtr<UInputAction>, FString>& Pair : InputActionToEmotiveTagMap)
    {
        UInputAction* InputAction = Pair.Key;
        if (!InputAction) continue;

        // Bind different trigger events to the same handler
        // You might want separate handlers for different granularities (e.g., Started vs Triggered)
        BoundInputHandles.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &UInputContextBridgeComponent::HandleInputActionStarted).Get</li>
<li>BoundInputHandles.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Triggered, this, &UInputContextBridgeComponent::HandleInputActionTriggered).Get</li>
<li>BoundInputHandles.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Completed, this, &UInputContextBridgeComponent::HandleInputActionCompleted).Get</li>
<li>BoundInputHandles.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Canceled, this, &UInputContextBridgeComponent::HandleInputActionCanceled).Get</li>
<li>// For analog inputs that send continuous values
        BoundInputHandles.Add(EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Ongoing, this, &UInputContextBridgeComponent::HandleInputActionOngoing).Get</li>
    }

    UE_LOG(LogTemp, Log, TEXT("[InputBridge] Bound %d input actions."), BoundInputHandles.Num());
}

void UInputContextBridgeComponent::UnbindInputActions()
{
    APlayerController* PlayerController = Cast<APlayerController>(GetOwner());
    if (!PlayerController) return;

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
    if (!EnhancedInputComponent) return;

    for (FInputBindingHandle Handle : BoundInputHandles)
    {
        EnhancedInputComponent->RemoveBindingbyHandle(Handle);
    }
    BoundInputHandles.Empty();
}

void UInputContextBridgeComponent::BroadcastEmotiveIntent(UInputAction* InputAction, float Intensity, FVector Direction, bool bTriggered)
{
    FEmotiveIntentAction Action;
    // Get the emotive tag from the map
    Action.ActionTag = InputActionToEmotiveTagMap.Contains(InputAction) ? InputActionToEmotiveTagMap[InputAction] : InputAction->GetName();
    Action.Intensity = Intensity;
    Action.Direction = Direction;
    Action.bIsTriggered = bTriggered;

    OnEmotiveIntentAction.Broadcast(Action);
    UE_LOG(LogTemp, VeryVerbose, TEXT("[InputBridge] Broadcasted Emotive Intent: %s (Int: %.2f, Dir: %s, Triggered: %s)"),
        *Action.ActionTag, Action.Intensity, *Action.Direction.ToString(), bTriggered ? TEXT("True") : TEXT("False"));
}

void UInputContextBridgeComponent::HandleInputActionTriggered(UInputAction* InputAction)
{
    FInputActionValue Value = InputAction->GetLastTriggeredValue();
    FVector Direction = FVector::ZeroVector;
    float Intensity = 1.0f;

    if (Value.GetMagnitudeSq() > 0.0f) // If it's an analog input
    {
        Direction = Value.Get<FVector>();
        Intensity = Direction.Size(); // Or use a specific component for intensity
    }
    
    BroadcastEmotiveIntent(InputAction, Intensity, Direction, true);
}

void UInputContextBridgeComponent::HandleInputActionCompleted(UInputAction* InputAction)
{
    // For release events, intensity is 0, bIsTriggered is false
    BroadcastEmotiveIntent(InputAction, 0.0f, FVector::ZeroVector, false);
}

void UInputContextBridgeComponent::HandleInputActionStarted(UInputAction* InputAction)
{
    // For initial press, intensity is 1, bIsTriggered is true
    BroadcastEmotiveIntent(InputAction, 1.0f, FVector::ZeroVector, true);
}

void UInputContextBridgeComponent::HandleInputActionCanceled(UInputAction* InputAction)
{
    // Canceled is similar to completed for most purposes, indicating end of action
    BroadcastEmotiveIntent(InputAction, 0.0f, FVector::ZeroVector, false);
}

void UInputContextBridgeComponent::HandleInputActionOngoing(UInputAction* InputAction)
{
    FInputActionValue Value = InputAction->GetLastTriggeredValue();
    FVector Direction = FVector::ZeroVector;
    float Intensity = 1.0f;

    if (Value.GetMagnitudeSq() > 0.0f) // If it's an analog input
    {
        Direction = Value.Get<FVector>();
        Intensity = Direction.Size();
    }
    // For ongoing actions, bIsTriggered might be false to distinguish from initial press
    BroadcastEmotiveIntent(InputAction, Intensity, Direction, false);
}
