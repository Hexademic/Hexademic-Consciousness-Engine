#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h"          // For FEmotionalState, FConsciousnessState
#include "Core/EmotionalArchetype.h" // For EEmotionalArchetype
#Components/HexademicConsciousnessComponent.h" // For UHexademicConsciousnessComponent
#include "AnimationBridgeComponent.generated.h"

// Forward Declaration for USkeletalMeshComponent
class USkeletalMeshComponent;
// Forward Declaration for UAnimInstance (if interacting directly with AnimBP)
class UAnimInstance;

UCLASS(ClassGroup=(HexademicBridge), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UAnimationBridgeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAnimationBridgeComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /**
     * @brief Sets the skeletal mesh component that this bridge will animate.
     * @param InMesh A pointer to the USkeletalMeshComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Animation Bridge")
    void SetTargetMesh(USkeletalMeshComponent* InMesh);

    /**
     * @brief Sets the linked Hexademic Consciousness Component to retrieve state from.
     * @param InConsciousness A pointer to the UHexademicConsciousnessComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Animation Bridge")
    void SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness);

    // Reference to the skeletal mesh to animate
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;

    // Reference to the consciousness component to read states from
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UHexademicConsciousnessComponent> LinkedConsciousness;

protected:
    // Animation blending parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Tuning")
    float EmotionalBlendSpeed = 5.0f; // Speed at which emotional animations blend
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Tuning")
    float PostureBlendSpeed = 2.0f; // Speed at which posture adjusts

    // Map Emotional Archetypes to specific animation montages or blend spaces
    // This would need to be populated in Blueprint or code with actual animation assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Mapping")
    TMap<EEmotionalArchetype, TObjectPtr<UAnimMontage>> EmotionalAnimMontages;

    // Optional: Morph target names for fine-grained facial expressions (similar to AvatarMotionLinkComponent)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation Mapping")
    FName FacialExpressionMorphTarget; // e.g., for general emotional intensity

    // Internal function to update animations based on consciousness state
    void UpdateAnimations(float DeltaTime);
    // Internal function to update posture based on consciousness state
    void UpdatePosture(float DeltaTime);
    // Internal function to trigger memory-based gestures (conceptual)
    void TriggerMemoryGestures();
};
