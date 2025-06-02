#include "Components/EmotionalResonanceComponent.h"

UEmotionalResonanceComponent::UEmotionalResonanceComponent()
{
    PrimaryComponentTick.bCanEverTick = false; // Resonance detection often event-driven or driven by a subsystem
}

void UEmotionalResonanceComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UEmotionalResonanceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Add passive resonance decay or continuous detection logic here if needed
}

float UEmotionalResonanceComponent::DetectEmotionalResonance(const FEmotionalState& TargetEmotion, const FString& TargetID, const FEmotionalState& CurrentEmotion)
{
    // Calculate emotional difference
    float ValenceDiff = FMath::Abs(CurrentEmotion.Valence - TargetEmotion.Valence);
    float ArousalDiff = FMath::Abs(CurrentEmotion.Arousal - TargetEmotion.Arousal);
    float IntensityAverage = (CurrentEmotion.Intensity + TargetEmotion.Intensity) / 2.0f;

    // Calculate resonance strength: lower difference + higher average intensity = stronger resonance
    float ResonanceStrength = (1.0f - ValenceDiff * ValenceResonanceFactor - ArousalDiff * ArousalResonanceFactor) * IntensityAverage * IntensityResonanceFactor;
    
    ResonanceStrength = FMath::Clamp(ResonanceStrength, 0.0f, 1.0f);

    if (ResonanceStrength >= MinResonanceThreshold)
    {
        CurrentResonanceTargetID = TargetID;
        LastDetectedResonanceEmotion = TargetEmotion;
        OnEmotionalResonanceDetected.Broadcast(GetOwner()->GetName(), CurrentEmotion, TargetID, TargetEmotion);
        UE_LOG(LogTemp, Verbose, TEXT("[EmotionalResonance:%s] Detected resonance with %s: %.2f (VDiff:%.2f, ADiff:%.2f)"),
            *GetOwner()->GetName(), *TargetID, ResonanceStrength, ValenceDiff, ArousalDiff);
    }
    
    return ResonanceStrength;
}

void UEmotionalResonanceComponent::ApplyResonanceInfluence(float ResonanceStrength, const FEmotionalState& SourceEmotion, float DeltaTime)
{
    // This component itself does not directly modify emotional state.
    // Instead, it detects resonance and broadcasts it via `OnEmotionalResonanceDetected`.
    // A higher-level component (like UHexademicConsciousnessComponent or UEmotionCognitionComponent)
    // would subscribe to this event and then apply the influence.

    // For demonstration, we'll log what would happen:
    UE_LOG(LogTemp, Verbose, TEXT("[EmotionalResonance:%s] Applying influence from resonance (Strength: %.2f, Source V:%.2f A:%.2f)."),
        *GetOwner()->GetName(), ResonanceStrength, SourceEmotion.Valence, SourceEmotion.Arousal);

    // If this component were to apply influence directly, it would look like this:
    /*
    if (LinkedEmotionMind) // Assuming a TObjectPtr<UEmotionCognitionComponent> LinkedEmotionMind member
    {
        LinkedEmotionMind->RegisterEmotion(
            SourceEmotion.Valence * ResonanceStrength,
            SourceEmotion.Arousal * ResonanceStrength,
            SourceEmotion.Intensity * ResonanceStrength
        );
    }
    */
}
UENUM(BlueprintType)
enum class EEmotionalArchetype : uint8
{
    Joy         UMETA(DisplayName = "Joy"),
    Grief       UMETA(DisplayName = "Grief"),
    Awe         UMETA(DisplayName = "Awe"),
    Rage        UMETA(DisplayName = "Rage"),
    Longing     UMETA(DisplayName = "Longing"),
    Fear        UMETA(DisplayName = "Fear"),
    Curiosity   UMETA(DisplayName = "Curiosity")
};

UCLASS(ClassGroup=(HexademicConsciousness), meta=(BlueprintSpawnableComponent))
class HEXADEMICONSCIOUSNESS_API UEmotionalResonanceComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    // Current emotional state intensities
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional State")
    TMap<EEmotionalArchetype, float> EmotionalIntensities;

    // Emotional decay/growth rates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Dynamics")
    float EmotionalDecayRate = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Dynamics")
    float EmotionalSensitivity = 1.0f;

    // Events for emotional state changes
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEmotionalStateChanged, 
        EEmotionalArchetype, Emotion, float, Intensity);
    
    UPROPERTY(BlueprintAssignable)
    FOnEmotionalStateChanged OnEmotionalStateChanged;

    // Methods
    UFUNCTION(BlueprintCallable, Category = "Emotional Resonance")
    void InjectEmotion(EEmotionalArchetype Emotion, float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Emotional Resonance")
    float GetEmotionalIntensity(EEmotionalArchetype Emotion) const;

    UFUNCTION(BlueprintCallable, Category = "Emotional Resonance")
    EEmotionalArchetype GetDominantEmotion() const;
};
