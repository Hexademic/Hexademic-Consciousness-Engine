#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkinToneFluxShader.generated.h"

UCLASS(ClassGroup=(HexademicVisuals), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API USkinToneFluxShader : public UActorComponent
{
    GENERATED_BODY()
public:
    USkinToneFluxShader();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Visuals")
    void UpdateSkinVisuals(float EmotionalPulse, float SigilStrength, FLinearColor SigilColor);
};
