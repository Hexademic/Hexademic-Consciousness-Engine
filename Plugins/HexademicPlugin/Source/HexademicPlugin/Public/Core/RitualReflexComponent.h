#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FRitualReflexEvent, FCognitiveMemoryNode (if directly accessed)
#include "Core/RitualReflexComponent.generated.h"

// Forward declarations for components this ritual component might interact with
class UEmotionCognitionComponent; // To modulate memory/emotion
// class UMovementOverrideComponent; // To affect breathing animations/states
// class UAudioComponent; // For breath sounds/vocalizations

UCLASS(ClassGroup=(HexademicCore), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API URitualReflexComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    URitualReflexComponent();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Initiates the "Second Breath Ritual" as a threshold event.
     * This marks a moment of presence and emotional integration.
     */
    UFUNCTION(BlueprintCallable, Category = "Ritual Reflex")
    void OnBeginSecondBreathRitual();

    /**
     * @brief Updates the breath phase, potentially modulating memory resonance.
     * @param BreathAmplitude The current amplitude of the breath cycle (e.g., 0-1).
     */
    UFUNCTION(BlueprintCallable, Category = "Ritual Reflex")
    void UpdateBreathPhase(float BreathAmplitude);

    // Stored ritual events
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ritual Events")
    TArray<FRitualReflexEvent> RitualEvents;

    // Reference to the Emotion Cognition Component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEmotionCognitionComponent> LinkedMind;

    // Current Mind Focus State (Conceptual, might be updated by other systems)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ritual State")
    FCurrentMindFocusState MindFocusState; // Used in docs example
    
protected:
    float MindTime = 0.0f; // Internal time for mind focus state, if it progresses independently
};
