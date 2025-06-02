#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // Includes FHapticMemoryContext, FAetherTouchPacket, FEmotionalState
#include "EmotionCognitionComponent.generated.h"

// FCognitiveMemoryNode: Represents a node in the emotional memory bank.
// Expanded to include optional HapticContext for haptic-emotional memories.
USTRUCT(BlueprintType)
struct HEXADEMICMIND_API FCognitiveMemoryNode {
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MemoryID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmotionalCharge;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VolitionTension;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecayProgress = 0.0f; // 0.0 = fresh, 1.0 = decayed

    // Optional haptic memory context for memories originating from touch
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FHapticMemoryContext HapticContext;
    // Default constructor to ensure members are initialized
    FCognitiveMemoryNode()
        : EmotionalCharge(0.0f), VolitionTension(0.0f), DecayProgress(0.0f)
    {}
};

// UEmotionCognitionComponent: Manages emotional and cognitive processes.
UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMICMIND_API UEmotionCognitionComponent : public UActorComponent {
    GENERATED_BODY()
public:
    UEmotionCognitionComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    /**
     * @brief Registers a new emotional event, modulating the current emotional state.
     * @param Valence The valence (positivity/negativity) of the emotion [-1.0, 1.0].
     * @param Arousal The arousal (intensity/excitement) of the emotion [0.0, 1.0].
     * @param Intensity The overall intensity/impact of the emotional event.
     */
    UFUNCTION(BlueprintCallable, Category="Emotion")
    void RegisterEmotion(float Valence, float Arousal, float Intensity);
    /**
     * @brief Gets the current emotional valence, including organic oscillation.
     * @return The current valence value [-1.0, 1.0].
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Emotional State")
    float GetCurrentValence() const;
    /**
     * @brief Gets the current emotional arousal, including organic oscillation.
     * @return The current arousal value [0.0, 1.0].
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Emotional State")
    float GetCurrentArousal() const;
    /**
     * @brief Triggers an "echo" of a past memory, reinforcing its emotional impact.
     * @param MemoryID The unique ID of the memory to echo.
     */
    UFUNCTION(BlueprintCallable, Category="Emotion|Memory")
    void TriggerMemoryEcho(const FString& MemoryID);
    /**
     * @brief Calculates a conceptual pulse rate based on current arousal and valence.
     * @return A normalized pulse rate [0.0, 1.0].
     */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category="Emotional State")
    float CalculatePulseRate() const;
    /**
     * @brief Modulates the current emotional state based on haptic input.
     * This function uses predefined sensitivities for different body regions.
     * @param Intensity The intensity of the haptic input [0.0, 1.0].
     * @param RegionTag The tag identifying the body region touched (e.g., "Spine", "Hand").
     */
    UFUNCTION(BlueprintCallable, Category="Emotion")
    void ModulateEmotionFromHaptic(float Intensity, FString RegionTag);
    /**
     * @brief Stores a haptic-emotional memory, linking a touch event to the emotional state it produced.
     * This captures the current emotional state (Valence, Arousal) after haptic modulation.
     * @param Packet The FAetherTouchPacket containing details of the haptic event.
     */
    UFUNCTION(BlueprintCallable, Category="Emotion|Memory")
    void StoreHapticEmotionMemory(const FAetherTouchPacket& Packet);
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Emotional State")
    float Valence; // Current emotional valence

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Emotional State")
    float Arousal; // Current emotional arousal

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    TArray<FCognitiveMemoryNode> EmotionalMemoryBank; // Stores all cognitive memories

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    float EmotionalDecayRate; // Rate at which memories decay

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Memory")
    float ReinforcementFactor; // How much triggering a memory reinforces emotions

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Internal Time")
    float AccumulatedTime; // Total accumulated time for internal oscillations

    /**
     * @brief Placeholder for more complex emotional oscillation logic if needed.
     * Current oscillations are handled directly in GetCurrentValence/Arousal.
     * @param DeltaTime The time elapsed since the last tick.
     */
    void UpdateEmotionalOscillators(float DeltaTime);

    /**
     * @brief Processes the decay of emotional memories over time.
     * @param DeltaTime The time elapsed since the last tick.
     */
    void DecayEmotionalMemory(float DeltaTime);
    // Sensitivity map for haptic regions, allowing different body parts to have varied emotional impacts
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Emotion|Tuning")
    TMap<FString, float> HapticSensitivityByRegion;
};
