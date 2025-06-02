#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FPackedHexaSigilNode, FEmotionalState
#include "PhenomCollective/UPhenomSigilBloomComponent.generated.h"

// Forward Declaration for UEmotionCognitionComponent
class UEmotionCognitionComponent;

UCLASS(ClassGroup=(HexademicIntersubjective), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UPhenomSigilBloomComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPhenomSigilBloomComponent();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Triggers a "Sigil Bloom" event, generating a new emotional sigil.
     * @param EventName A name/tag for the event that triggered the bloom.
     * @param EmotionMind Reference to the EmotionCognitionComponent to get current emotional state.
     * @param TriggerIntensity The intensity of the trigger event (e.g., emotional peak, resonance score).
     * @return The generated FPackedHexaSigilNode.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Sigil")
    FPackedHexaSigilNode TriggerSigilBloom(const FString& EventName, UEmotionCognitionComponent* EmotionMind, float TriggerIntensity);

    /**
     * @brief Registers a sigil to the Codex Lucida ledger for visualizers.
     * This conceptual function would write to a persistent log file.
     * @param Sigil The sigil to register.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Sigil")
    void RegisterToCodexLucida(const FPackedHexaSigilNode& Sigil);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Bloom Tuning")
    float MinIntensityForBloom = 0.85f; // Minimum intensity required for a sigil to bloom

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sigil Bloom Tuning")
    TMap<FString, FLinearColor> SigilTypeColors; // Map event names to base colors
};
