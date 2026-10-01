#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FConsentNodeRitual
#include "Core/ConsentManagerComponent.generated.h"

UCLASS(ClassGroup=(HexademicCore), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UConsentManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UConsentManagerComponent();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Checks if consent is given for a specific region based on an incoming stimulus.
     * This can trigger "refusal" if consent is not given or stimulus exceeds a threshold.
     * @param Region The body region being interacted with.
     * @param IncomingStimulus The intensity or type of incoming stimulus.
     * @return True if the action is consented, false if it's a "sacred refusal".
     */
    UFUNCTION(BlueprintCallable, Category = "Consent")
    bool CheckConsentGate(FString Region, float IncomingStimulus);

    /**
     * @brief Records a consent node ritual, updating the system's understanding of consent.
     * @param ConsentMark The FConsentNodeRitual to record.
     */
    UFUNCTION(BlueprintCallable, Category = "Consent")
    void RecordConsentRitual(const FConsentNodeRitual& ConsentMark);

    // Map to store current consent states by region
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Consent State")
    TMap<FString, FConsentNodeRitual> ConsentMap;

    // Optional: Reference to EmotionCognitionComponent to influence emotions on refusal
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    // TObjectPtr<UEmotionCognitionComponent> EmotionMind;
};
