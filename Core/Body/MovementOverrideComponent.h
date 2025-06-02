#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Body/MovementOverrideComponent.generated.h"

class UCharacterMovementComponent; // Forward declare CharacterMovementComponent

UCLASS(ClassGroup=(HexademicBody), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UMovementOverrideComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMovementOverrideComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /**
     * @brief Initiates a physics-driven movement override (e.g., for climbing, vaulting).
     * @param OverrideType The type of movement override (e.g., "Climb", "Vault", "Grapple").
     * @param Duration The duration of the override.
     * @param TargetLocation Optional: The target location for the movement.
     */
    UFUNCTION(BlueprintCallable, Category = "Movement Override")
    void ActivateMovementOverride(FString OverrideType, float Duration, FVector TargetLocation = FVector::ZeroVector);

    /**
     * @brief Deactivates any active movement override.
     */
    UFUNCTION(BlueprintCallable, Category = "Movement Override")
    void DeactivateMovementOverride();

    // Reference to the character's movement component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UCharacterMovementComponent> CharacterMovement;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Override State")
    bool bIsMovementOverridden = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Override State")
    FString ActiveOverrideType;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Override State")
    float OverrideRemainingTime = 0.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Override State")
    FVector OverrideTargetLocation = FVector::ZeroVector;

    void ApplyRootMotionOverride(float DeltaTime);
    void RestoreOriginalMovementMode();
};
