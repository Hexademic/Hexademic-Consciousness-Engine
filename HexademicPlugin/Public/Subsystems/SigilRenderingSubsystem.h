#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Core/SigilProjection.h" // For FSigilProjection
#include "Subsystems/SigilRenderingSubsystem.generated.h"

// Forward Declarations
class USigilProjectionComponent; // To trigger individual sigil projections
class USkinToneFluxShader;       // For global skin effects
class UHexademicHolographicCode; // For global holographic displays

/**
 * @brief Manages global real-time sigil generation and rendering.
 * This subsystem handles consciousness state visualization,
 * emotional aura effects, and quantum resonance field displays,
 * often coordinating with individual SigilProjectionComponents.
 */
UCLASS()
class HEXADEMICPLUGIN_API USigilRenderingSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;
    virtual void Tick(float DeltaTime) override; // For continuous global rendering updates

    // --- Global Sigil & Aura Management ---
    /**
     * @brief Triggers a global sigil projection, visible across the world.
     * @param SigilData The data defining the global sigil.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Rendering")
    void TriggerGlobalSigilDisplay(const FSigilProjection& SigilData);

    /**
     * @brief Updates global emotional aura effects based on the world's emotional state.
     * @param GlobalEmotion The current aggregated global emotional state.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Rendering")
    void UpdateGlobalEmotionalAura(const FEmotionalState& GlobalEmotion);

    /**
     * @brief Updates the global quantum resonance field visualization.
     * @param GlobalQuantumState The aggregated global quantum state.
     */
    UFUNCTION(BlueprintCallable, Category = "Sigil Rendering")
    void UpdateGlobalQuantumField(const FQuantumAnalogState& GlobalQuantumState);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering Tuning")
    float GlobalDisplayUpdateFrequency = 10.0f; // Hz for global visual updates

    // Storage for active global sigils (if they persist)
    UPROPERTY()
    TMap<FString, FSigilProjection> ActiveGlobalSigils;

    // References to global visual components/actors in the world
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<AActor> GlobalSigilDisplayActor; // An actor that might host global visual effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UMaterialInstanceDynamic> GlobalAuraMaterial; // Material for world-wide emotional aura

    // Internal timer for update frequency
    float AccumulatedGlobalDisplayTime = 0.0f;

    // Helper functions for updating specific global visual effects
    void ProcessGlobalSigils(float DeltaTime);
    void UpdateGlobalAuraEffect(const FEmotionalState& CurrentGlobalEmotion);
    void UpdateGlobalQuantumEffect(const FQuantumAnalogState& CurrentGlobalQuantumState);
};
