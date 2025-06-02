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
