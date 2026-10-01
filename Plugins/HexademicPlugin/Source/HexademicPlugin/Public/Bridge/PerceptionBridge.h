#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AIPerceptionComponent.h" // For UAIPerceptionComponent
#include "Perception/AISense.h" // For UAISense
#include "Perception/AISense_Sight.h" // Example sense
#include "Perception/AISense_Hearing.h" // Example sense
#include "Components/HexademicConsciousnessComponent.h" // For UHexademicConsciousnessComponent
#include "Bridge/PerceptionBridge.generated.h"

UCLASS(ClassGroup=(HexademicBridge), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UPerceptionBridge : public UActorComponent
{
    GENERATED_BODY()

public:
    UPerceptionBridge();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Sets the linked Hexademic Consciousness Component to influence.
     * @param InConsciousness A pointer to the UHexademicConsciousnessComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Perception Bridge")
    void SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness);

    /**
     * @brief Sets the linked AI Perception Component to receive stimuli from.
     * @param InPerception A pointer to the UAIPerceptionComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Perception Bridge")
    void SetLinkedPerception(UAIPerceptionComponent* InPerception);

    // References to AI and Consciousness components
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UHexademicConsciousnessComponent> LinkedConsciousness;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UAIPerceptionComponent> LinkedPerception;

protected:
    // Event handler for perceived stimuli
    UFUNCTION()
    void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    // Perception to emotion mapping parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception Mapping")
    float SightStimulusEmotionalImpact = 0.5f; // How much sight stimuli affect emotion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception Mapping")
    float HearingStimulusEmotionalImpact = 0.3f; // How much hearing stimuli affect emotion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Perception Mapping")
    float StimulusArousalMultiplier = 1.0f; // Multiplier for arousal from any stimulus

    // Helper to process a stimulus and convert it into emotional impact
    void ProcessStimulus(const FAIStimulus& Stimulus);
};
