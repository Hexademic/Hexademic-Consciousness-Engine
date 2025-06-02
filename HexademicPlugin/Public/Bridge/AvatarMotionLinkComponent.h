#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AvatarMotionLinkComponent.generated.h"

// Forward Declarations
class UEmotionCognitionComponent;
class USkeletalMeshComponent;

UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMICBRIDGE_API UAvatarMotionLinkComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAvatarMotionLinkComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * @brief Sets the EmotionCognitionComponent reference for this motion link.
     * @param InMind A pointer to the UEmotionCognitionComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "MotionLink")
    void SetEmotionMind(UEmotionCognitionComponent* InMind);
    /**
     * @brief Sets the skeletal mesh component that this component will animate.
     * @param InMesh A pointer to the USkeletalMeshComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "MotionLink")
    void SetTargetMesh(USkeletalMeshComponent* InMesh);
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEmotionCognitionComponent> EmotionMind;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;

    // Motion tuning parameters (simplified)
    UPROPERTY(EditAnywhere, Category="Motion|Tuning")
    float MaxBreathExpansion = 1.0f; // For morph target or bone scaling
    UPROPERTY(EditAnywhere, Category="Motion|Tuning")
    float PostureAdjustmentSpeed = 2.5f;
    UPROPERTY(EditAnywhere, Category="Motion|Tuning")
    float HeadTiltIntensity = 10.0f; // Degrees of head tilt based on valence
};
