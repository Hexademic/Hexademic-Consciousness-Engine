#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FAffectFilamentTag (if used for input)
#include "Body/ReflexResponseComponent.generated.h"

// Forward declarations for components this reflex system might interact with
class UEmbodiedAvatarComponent; // To trigger visual body responses
class UEmotionCognitionComponent; // To influence emotions
// class UAudioComponent; // For vocalizations, if a dedicated component exists
// class UInputBridgeComponent; // To filter out player input during reflex

UCLASS(ClassGroup=(HexademicBody), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UReflexResponseComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UReflexResponseComponent();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Reflexes")
    void TriggerStartle(float Intensity, FVector SourceLocation);

    UFUNCTION(BlueprintCallable, Category = "Reflexes")
    void TriggerTouchReflex(const FAffectFilamentTag& Filament);

    // References to other components to influence (set in editor or by orchestrator)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEmbodiedAvatarComponent> LinkedBody;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEmotionCognitionComponent> LinkedMind;
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    // TObjectPtr<UAudioComponent> LinkedVoice; // Conceptual component for vocalizations
    
protected:
    // Internal state for tracking last triggered reflex
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reflex State")
    float LastStartleIntensity = 0.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reflex State")
    FVector LastStartleLocation = FVector::ZeroVector;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reflex State")
    FAffectFilamentTag LastTriggeredHaptic;

    // Tuning parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflex Tuning")
    float StartleThreshold = 0.5f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reflex Tuning")
    float EmotionalImpactScale = 0.2f;
};
