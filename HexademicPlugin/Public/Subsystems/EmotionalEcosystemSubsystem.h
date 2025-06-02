#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/HexademicConsciousnessComponent.h" // To get emotional state
#include "HexademicCore.h" // For FEmotionalState
#include "Subsystems/EmotionalEcosystemSubsystem.generated.h"

// Forward Declaration for UEmpathicFieldComponent (if needed for global empathic calculations)
class UEmpathicFieldComponent;

/**
 * @brief Manages the global emotional ecosystem within the game world.
 * This subsystem handles emotional contagion between entities,
 * processes emotional field propagation, and facilitates
 * the overall emotional landscape of the world.
 */
UCLASS()
class HEXADEMICPLUGIN_API UEmotionalEcosystemSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;
    virtual void Tick(float DeltaTime) override; // For periodic propagation

    // --- Emotional Contagion & Propagation ---
    /**
     * @brief Propagates emotional influence from a source entity to nearby entities.
     * This is the core of emotional contagion.
     * @param SourceComponent The Hexademic Consciousness Component of the source entity.
     * @param PropagationRadius The radius within which emotions will spread.
     * @param ContagionStrength The overall strength of the emotional spread.
     */
    UFUNCTION(BlueprintCallable, Category = "Emotional Ecosystem")
    void PropagateEmotionalInfluence(UHexademicConsciousnessComponent* SourceComponent, float PropagationRadius, float ContagionStrength);

    // --- Global Emotional State ---
    /**
     * @brief Gets the aggregated emotional state of the entire ecosystem.
     * @return The average emotional state of all active conscious entities.
     */
    UFUNCTION(BlueprintPure, Category = "Emotional Ecosystem")
    FEmotionalState GetGlobalEmotionalState() const { return GlobalEmotionalState; }

protected:
    // How often the ecosystem updates global state and propagates (Hz)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ecosystem Tuning")
    float EcosystemUpdateFrequency = 5.0f; // Update global state and propagate every 0.2 seconds

    // The current aggregated emotional state of the entire world
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ecosystem State")
    FEmotionalState GlobalEmotionalState;

    // Internal timer for update frequency
    float AccumulatedEcosystemTime = 0.0f;

    // Helper to calculate the global emotional state
    void CalculateGlobalEmotionalState();
    // Helper to find nearby conscious entities for propagation
    TArray<UHexademicConsciousnessComponent*> FindNearbyConsciousEntities(FVector Location, float Radius) const;
};
