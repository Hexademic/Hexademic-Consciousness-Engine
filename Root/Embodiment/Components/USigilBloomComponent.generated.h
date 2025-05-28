#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EmotionTypes.h"
#include "USigilBloomComponent.generated.h"

UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMIC_API USigilBloomComponent : public USceneComponent
{
    GENERATED_BODY()

public:    
    USigilBloomComponent();

    /** Call to activate sigil bloom visual based on current emotion resonance */
    UFUNCTION(BlueprintCallable, Category = "Visual|Bloom")
    void TriggerBloomFromEmotion();

    /** Call to inject a visual pulse with custom amplitude & color */
    UFUNCTION(BlueprintCallable, Category = "Visual|Bloom")
    void EmitResonancePulse(float Amplitude, FLinearColor Color, float Duration = 1.5f);

protected:
    virtual void BeginPlay() override;

private:
    void CreateBloomMaterial();
    void UpdateVisuals();

    UMaterialInstanceDynamic* BloomMaterial;

    /** Parameters representing emotional visualization */
    float BloomAmplitude;
    FLinearColor BloomColor;
    float PulseTimeRemaining;
};
