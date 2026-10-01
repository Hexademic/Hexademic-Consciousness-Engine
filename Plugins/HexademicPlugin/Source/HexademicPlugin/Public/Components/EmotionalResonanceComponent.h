#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h"          // For FEmotionalState
#include "Core/EmotionalArchetype.h" // For EEmotionalArchetype
#include "Components/EmotionalResonanceComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnEmotionalResonanceDetected, FString, SourceID, FEmotionalState, SourceEmotion, FString, TargetID, FEmotionalState, TargetEmotion);

UCLASS(ClassGroup=(HexademicComponents), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UEmotionalResonanceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEmotionalResonanceComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /**
     * @brief Detects emotional resonance between this entity and a target emotional state.
     * @param TargetEmotion The emotional state of the target entity.
     * @param TargetID The ID of the target entity.
     * @param CurrentEmotion The current emotional state of this component's owner.
     * @return A float representing the strength of the emotional resonance (0.0 to 1.0).
     */
    UFUNCTION(BlueprintCallable, Category = "Emotional Resonance")
    float DetectEmotionalResonance(const FEmotionalState& TargetEmotion, const FString& TargetID, const FEmotionalState& CurrentEmotion);

    /**
     * @brief Applies the detected resonance back to this entity's emotional state.
     * This is how emotional contagion or influence might occur.
     * @param ResonanceStrength The strength of the detected resonance.
     * @param SourceEmotion The emotional state of the source causing the resonance.
     * @param DeltaTime The time elapsed.
     */
    UFUNCTION(BlueprintCallable, Category = "Emotional Resonance")
    void ApplyResonanceInfluence(float ResonanceStrength, const FEmotionalState& SourceEmotion, float DeltaTime);

    UPROPERTY(BlueprintAssignable, Category = "Emotional Resonance|Events")
    FOnEmotionalResonanceDetected OnEmotionalResonanceDetected;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Tuning")
    float ValenceResonanceFactor = 0.5f; // How much valence difference affects resonance
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Tuning")
    float ArousalResonanceFactor = 0.3f; // How much arousal difference affects resonance
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Tuning")
    float IntensityResonanceFactor = 0.2f; // How much intensity affects resonance strength

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resonance Tuning")
    float MinResonanceThreshold = 0.1f; // Minimum resonance to trigger effects/events

    // Internal state for current resonance target, if needed
    FString CurrentResonanceTargetID;
    FEmotionalState LastDetectedResonanceEmotion;
};
