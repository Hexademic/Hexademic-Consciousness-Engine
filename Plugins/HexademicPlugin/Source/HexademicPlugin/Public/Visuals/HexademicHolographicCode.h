#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicHolographicCode.generated.h"

UCLASS(ClassGroup=(HexademicVisuals), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UHexademicHolographicCode : public UActorComponent
{
    GENERATED_BODY()
public:
    UHexademicHolographicCode();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Visuals")
    void VisualizeSystemCoherence(float Coherence);
};
