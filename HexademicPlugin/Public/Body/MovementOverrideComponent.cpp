#include "Body/MovementOverrideComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UMovementOverrideComponent::UMovementOverrideComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMovementOverrideComponent::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to get CharacterMovementComponent from owner if not set
    if (!CharacterMovement)
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            CharacterMovement = OwnerCharacter->GetCharacterMovement();
        }
    }

    if (!CharacterMovement)
    {
        UE_LOG(LogTemp, Warning, TEXT("[MovementOverride] CharacterMovementComponent not found on owner. Movement overrides will not function."));
    }
}

void UMovementOverrideComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsMovementOverridden && CharacterMovement)
    {
        ApplyRootMotionOverride(DeltaTime);
        OverrideRemainingTime -= DeltaTime;

        if (OverrideRemainingTime <= 0.0f)
        {
            DeactivateMovementOverride();
        }
    }
}

void UMovementOverrideComponent::ActivateMovementOverride(FString OverrideType, float Duration, FVector TargetLocation)
{
    if (!CharacterMovement)
    {
        UE_LOG(LogTemp, Error, TEXT("[MovementOverride] Cannot activate override: CharacterMovementComponent is null."));
        return;
    }

    bIsMovementOverridden = true;
    ActiveOverrideType = OverrideType;
    OverrideRemainingTime = Duration;
    OverrideTargetLocation = TargetLocation;

    // Disable standard movement input and set movement mode for root motion
    CharacterMovement->SetMovementMode(MOVE_Custom); // Or MOVE_Flying, depending on needs
    CharacterMovement->StopMovementImmediately(); // Stop current movement

    UE_LOG(LogTemp, Log, TEXT("[MovementOverride] Activated '%s' override for %.2f seconds."), *OverrideType, Duration);
}

void UMovementOverrideComponent::DeactivateMovementOverride()
{
    if (bIsMovementOverridden)
    {
        bIsMovementOverridden = false;
        ActiveOverrideType = TEXT("");
        OverrideRemainingTime = 0.0f;
        RestoreOriginalMovementMode();
        UE_LOG(LogTemp, Log, TEXT("[MovementOverride] Deactivated movement override."));
    }
}

void UMovementOverrideComponent::ApplyRootMotionOverride(float DeltaTime)
{
    // This is where you would apply your custom root motion logic.
    // For example, for a "Climb" override, you might:
    // 1. Calculate a desired new position/rotation based on animation and TargetLocation.
    // 2. Directly move the character's root.
    // 3. Ensure collision is handled.

    if (CharacterMovement && GetOwner())
    {
        FVector CurrentLocation = GetOwner()->GetActorLocation();
        FRotator CurrentRotation = GetOwner()->GetActorRotation();

        // Simple example: Lerp towards TargetLocation for a "Vault" or "Grapple"
        if (!OverrideTargetLocation.IsZero())
        {
            FVector NewLocation = FMath::VInterpTo(CurrentLocation, OverrideTargetLocation, DeltaTime, 5.0f);
            GetOwner()->SetActorLocation(NewLocation, true); // Sweep for collision
        }

        // More complex logic would involve playing specific root motion animations
        // GetOwner()->PlayAnimMontage(MyRootMotionMontage);
        // CharacterMovement->SetMovementMode(MOVE_Flying); // Often used for root motion
        // CharacterMovement->SetRootMotionSource(); // For more advanced root motion setups
    }
}

void UMovementOverrideComponent::RestoreOriginalMovementMode()
{
    if (CharacterMovement)
    {
        // Restore previous movement mode, e.g., walking
        CharacterMovement->SetMovementMode(MOVE_Walking);
    }
}
