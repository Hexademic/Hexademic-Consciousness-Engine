#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h"          // For FEmotionalState
#include "Core/EmotionalArchetype.h" // For EEmotionalArchetype
#include "Components/MemoryThreadComponent.h" // For FMemoryThread
#include "Components/PersonalityLayerComponent.generated.h"

// Forward declaration for UHexademicConsciousnessComponent (to get overall state)
class UHexademicConsciousnessComponent;

// FPersonalityTrait: Represents an emergent personality trait
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FPersonalityTrait
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Trait")
    FString TraitName; // e.g., "Optimistic", "Cautious", "Adventurous"
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Trait")
    float Strength; // How strong this trait is (0.0-1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Trait")
    FDateTime EmergenceTimestamp; // When this trait became significant
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Trait")
    TArray<FString> InfluencingMemoryThreadIDs; // Memories that shaped this trait
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Trait")
    EEmotionalArchetype AssociatedArchetype; // Dominant archetype associated with this trait
};


UCLASS(ClassGroup=(HexademicComponents), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UPersonalityLayerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPersonalityLayerComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /**
     * @brief Evolves the entity's personality based on its current consciousness state and memory threads.
     * This function drives the dynamic development of personality traits.
     * @param CurrentConsciousnessState The current high-level consciousness state.
     * @param MemoryThreads The current active memory threads.
     */
    UFUNCTION(BlueprintCallable, Category = "Personality")
    void EvolvePersonality(const FConsciousnessState& CurrentConsciousnessState, const TArray<FMemoryThread>& MemoryThreads);

    /**
     * @brief Retrieves the strength of a specific personality trait.
     * @param TraitName The name of the trait to query.
     * @return The strength of the trait (0.0-1.0).
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Personality")
    float GetTraitStrength(const FString& TraitName) const;

    // The set of emergent personality traits for this entity
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Personality Traits")
    TArray<FPersonalityTrait> EmergentPersonalityTraits;

    // Reference to the main Consciousness Component (for overall state)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UHexademicConsciousnessComponent> LinkedConsciousness;
    // Reference to the Memory Thread Component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UMemoryThreadComponent> LinkedMemoryThreads;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Tuning")
    float PersonalityEvolutionRate = 0.05f; // How fast personality traits evolve
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personality Tuning")
    float TraitDecayRate = 0.01f; // How fast unused traits decay

    // Internal helper to update/create a trait based on emotional archetype and influencing memories
    void UpdateOrCreateTrait(FPersonalityTrait& Trait, EEmotionalArchetype Archetype, const TArray<FString>& InfluencingMemories, float ImpactStrength, float DeltaTime);
    FPersonalityTrait* FindTraitByName(const FString& TraitName);
};
