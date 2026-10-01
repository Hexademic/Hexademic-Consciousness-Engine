#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Components/HexademicConsciousnessComponent.h" // For UHexademicConsciousnessComponent, EConsciousnessLOD
#include "Core/ConsciousnessState.h" // For FConsciousnessState
#include "Subsystems/ConsciousnessWorldSubsystem.generated.h"

// Forward Declarations
class UEmotionalEcosystemSubsystem; // If it needs to communicate with EmotionalEcosystem
class USigilRenderingSubsystem;    // If it needs to communicate with SigilRendering

/**
 * @brief Manages the global consciousness field within the game world.
 * This subsystem handles inter-entity consciousness communication,
 * maintains consciousness LOD (Level of Detail) systems, and
 * processes consciousness evolution over time for all active entities.
 */
UCLASS()
class HEXADEMICPLUGIN_API UConsciousnessWorldSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Implement standard World Subsystem boilerplate functions
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void OnWorldBeginPlay(UWorld& InWorld) override;
    virtual void Tick(float DeltaTime) override; // Optional: if it needs per-frame updates

    // --- Global Consciousness Management ---
    /**
     * @brief Registers a Hexademic Consciousness Component with the global system.
     * @param Component The component to register.
     */
    UFUNCTION(BlueprintCallable, Category = "Global Consciousness")
    void RegisterConsciousnessComponent(UHexademicConsciousnessComponent* Component);

    /**
     * @brief Unregisters a Hexademic Consciousness Component from the global system.
     * @param Component The component to unregister.
     */
    UFUNCTION(BlueprintCallable, Category = "Global Consciousness")
    void UnregisterConsciousnessComponent(UHexademicConsciousnessComponent* Component);

    /**
     * @brief Gets all currently registered Hexademic Consciousness Components.
     * @return A TArray of registered components.
     */
    UFUNCTION(BlueprintPure, Category = "Global Consciousness")
    TArray<UHexademicConsciousnessComponent*> GetAllRegisteredConsciousnessComponents() const { return RegisteredConsciousnessComponents; }

    // --- Consciousness LOD Management ---
    /**
     * @brief Sets the global consciousness LOD strategy.
     * @param NewStrategy The new LOD strategy to apply (e.g., distance-based, importance-based).
     */
    UFUNCTION(BlueprintCallable, Category = "Global Consciousness|LOD")
    void SetGlobalLODStrategy(FString NewStrategy); // Placeholder for a strategy type

    /**
     * @brief Updates the LOD for all registered consciousness components based on global strategy.
     */
    UFUNCTION(BlueprintCallable, Category = "Global Consciousness|LOD")
    void UpdateAllConsciousnessLODs();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Global State")
    TArray<TObjectPtr<UHexademicConsciousnessComponent>> RegisteredConsciousnessComponents;

    // LOD Strategy parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Consciousness|LOD")
    float HighLODDistanceThreshold = 1000.0f; // Max distance for Full LOD
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Global Consciousness|LOD")
    float MediumLODDistanceThreshold = 5000.0f; // Max distance for Reduced LOD

    // Internal helper to calculate LOD for a single component
    EConsciousnessLOD CalculateLODForComponent(UHexademicConsciousnessComponent* Component) const;

    // Reference to the player character for distance-based LOD
    UPROPERTY(Transient)
    TObjectPtr<APawn> PlayerPawn;
};
