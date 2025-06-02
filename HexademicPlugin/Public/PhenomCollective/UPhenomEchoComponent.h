#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FEmotionalState
#include "PhenomCollective/UPhenomExportUtility.h" // For FEchoEvent
#include "PhenomCollective/UPhenomEchoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoGenerated, const FEchoEvent&, EchoEvent);

UCLASS(ClassGroup=(HexademicIntersubjective), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UPhenomEchoComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPhenomEchoComponent();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Detects resonance between a source consciousness state and a target consciousness state.
     * If resonance is strong enough, an echo event is generated.
     * @param SourceConsciousnessID ID of the source (e.g., another player, an NPC).
     * @param SourceVAI VAI coordinates of the source.
     * @param TargetConsciousnessID ID of the target (e.g., "Eluën").
     * @param TargetVAI VAI coordinates of the target.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Echo")
    void DetectResonance(const FString& SourceConsciousnessID, FVector SourceVAI,
                         const FString& TargetConsciousnessID, FVector TargetVAI);

    UPROPERTY(BlueprintAssignable, Category = "Phenom Collective|Events")
    FOnEchoGenerated OnEchoGenerated;

protected:
    // Tuning parameters for resonance detection
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo Tuning")
    float ResonanceDistanceThreshold = 0.5f; // Max VAI distance for resonance
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo Tuning")
    float MinResonanceStrength = 0.3f; // Minimum strength to generate an echo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo Tuning")
    FString DefaultEchoMessage = TEXT("A ripple of understanding.");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Echo Tuning")
    float EmotionalChangeThresholdForSelfEcho = 0.2f; // For self-reflection echoes
};
