#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputCoreTypes.h" // For FKey, EInputEvent
#include "EnhancedInputSubsystems.h" // For Enhanced Input System (UE5+)
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/InputMappingContext.h"
#include "Core/InputContextBridgeComponent.generated.h"

// Enum to represent different input modes/contexts
UENUM(BlueprintType)
enum class EInputModeState : uint8
{
    Combat    UMETA(DisplayName = "Combat Mode"),
    Traversal UMETA(DisplayName = "Traversal Mode"),
    Interaction UMETA(DisplayName = "Interaction Mode"),
    SliceOfLife UMETA(DisplayName = "Slice of Life / AR Companion Mode"),
    UI        UMETA(DisplayName = "UI Mode")
};

// Struct to represent a normalized emotive-intent action
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FEmotiveIntentAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotive Intent")
    FString ActionTag; // e.g., "Intent_Comfort", "Intent_Strike", "Intent_Grapple"

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotive Intent")
    float Intensity = 1.0f; // 0.0-1.0, derived from analog input or duration

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotive Intent")
    FVector Direction = FVector::ZeroVector; // For directional inputs

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotive Intent")
    bool bIsTriggered = false; // True for a single press/release, false if continuous
};

// Delegate to broadcast normalized emotive-intent actions
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmotiveIntentAction, const FEmotiveIntentAction&, Action);

UCLASS(ClassGroup=(HexademicCore), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UInputContextBridgeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInputContextBridgeComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
    /**
     * @brief Switches the active input mode, applying relevant mapping contexts.
     * @param NewMode The new input mode to activate.
     */
    UFUNCTION(BlueprintCallable, Category = "Input Bridge")
    void SetInputMode(EInputModeState NewMode);

    // Delegate for broadcasting normalized actions
    UPROPERTY(BlueprintAssignable, Category = "Input Bridge|Events")
    FOnEmotiveIntentAction OnEmotiveIntentAction;

    // === Enhanced Input Mappings ===
    // Map of Input Mode to their corresponding Input Mapping Contexts
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mappings")
    TMap<EInputModeState, TObjectPtr<UInputMappingContext>> InputMappingContexts;

    // Map of Input Actions to their normalized FEmotiveIntentAction tags
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Mappings")
    TMap<TObjectPtr<UInputAction>, FString> InputActionToEmotiveTagMap;

    // Current active input mode
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input State")
    EInputModeState CurrentInputMode = EInputModeState::UI; // Default to UI mode

protected:
    // Internal helper to bind input actions to delegate
    void BindInputActions();
    void UnbindInputActions();

    // Callback functions for Enhanced Input Actions
    void HandleInputActionTriggered(UInputAction* InputAction);
    void HandleInputActionCompleted(UInputAction* InputAction);
    void HandleInputActionStarted(UInputAction* InputAction);
    void HandleInputActionCanceled(UInputAction* InputAction);
    void HandleInputActionOngoing(UInputAction* InputAction); // For analog inputs

    // Helper to create and broadcast an FEmotiveIntentAction
    void BroadcastEmotiveIntent(UInputAction* InputAction, float Intensity, FVector Direction, bool bTriggered);

    // Storing bound delegates to unbind them later
    TArray<FInputBindingHandle> BoundInputHandles;
};
