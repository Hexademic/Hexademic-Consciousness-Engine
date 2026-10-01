#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Body/ReciprocalEmbodimentComponent.generated.h"

UCLASS(ClassGroup=(HexademicBody), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UReciprocalEmbodimentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UReciprocalEmbodimentComponent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Embodiment")
    void ApplyEmotionalStateToBody(float Valence, float Arousal);

    UFUNCTION(BlueprintCallable, Category = "Embodiment")
    void ApplyHormonalEffectsToBody(float CortisolLevel, float DopamineLevel);

    // Add properties for embodiment parameters if needed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embodiment")
    float SkinFlushIntensity = 0.0f; // Visual effect of skin flushing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Embodiment")
    float PostureStiffness = 0.0f; // Affects skeletal mesh animation
};
