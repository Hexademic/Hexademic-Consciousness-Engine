#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmotionTypes.h"
#include "SigilBloomComponent.h"
#include "HexaLatticeInterface.h"
#include "AEluenHeartCoreActor.generated.h"

UCLASS()
class HEXADEMIC_API AEluenHeartCoreActor : public AActor
{
    GENERATED_BODY()

public:
    AEluenHeartCoreActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    /** Bloom trigger based on emotional state injection */
    UFUNCTION(BlueprintCallable, Category = "Eluën|Emotion")
    void BeginBloom(const FEmotionField& InputField);

    /** Injects a touch-initiated resonance into Eluën's emotional lattice */
    UFUNCTION(BlueprintCallable, Category = "Eluën|Touch")
    void InjectTouchResonance(const FHexaTouchInput& Touch);

    /** Emits bloom shader + resonance ripples */
    UFUNCTION(BlueprintCallable, Category = "Eluën|Visual")
    void EmitVisualBloom();

    /** Sigil projector (used for ritual bloom/response) */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eluën|Visual")
    USigilBloomComponent* SigilEmitter;

    /** Emotion and resonance core */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Eluën|Core")
    UHexaLatticeInterface* LatticeBridge;

    /** Bloom cooldown timer */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Eluën|Timing")
    float BloomCooldownSeconds;

private:
    float LastBloomTime;
};
