#include "Bridge/AnimationBridgeComponent.h"
#include "GameFramework/Character.h" // For ACharacter and GetMesh()
#include "Components/HexademicConsciousnessComponent.h" // To get consciousness state
#include "Animation/AnimInstance.h" // To access AnimInstance properties

UAnimationBridgeComponent::UAnimationBridgeComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UAnimationBridgeComponent::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to find TargetMesh and LinkedConsciousness on owner if not set
    if (!TargetMesh)
    {
        ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
        if (OwnerCharacter)
        {
            TargetMesh = OwnerCharacter->GetMesh();
        }
        if (!TargetMesh) UE_LOG(LogTemp, Warning, TEXT("[AnimationBridge] TargetMesh not found on owner %s."), *GetOwner()->GetName());
    }

    if (!LinkedConsciousness)
    {
        LinkedConsciousness = GetOwner()->FindComponentByClass<UHexademicConsciousnessComponent>();
        if (!LinkedConsciousness) UE_LOG(LogTemp, Warning, TEXT("[AnimationBridge] LinkedConsciousness not found on owner %s."), *GetOwner()->GetName());
    }
}

void UAnimationBridgeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!TargetMesh || !LinkedConsciousness) return;

    UpdateAnimations(DeltaTime);
    UpdatePosture(DeltaTime);
    TriggerMemoryGestures(); // This would be event-driven in a full system
}

void UAnimationBridgeComponent::SetTargetMesh(USkeletalMeshComponent* InMesh)
{
    TargetMesh = InMesh;
}

void UAnimationBridgeComponent::SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness)
{
    LinkedConsciousness = InConsciousness;
}

void UAnimationBridgeComponent::UpdateAnimations(float DeltaTime)
{
    if (!TargetMesh || !LinkedConsciousness) return;

    FConsciousnessState CurrentState = LinkedConsciousness->GetConsciousnessState();
    UAnimInstance* AnimInstance = TargetMesh->GetAnimInstance();

    if (!AnimInstance) return;

    // --- Emotional State → Animation Mapping ---
    UAnimMontage* TargetMontage = EmotionalAnimMontages.FindRef(CurrentState.DominantEmotionalArchetype);

    if (TargetMontage)
    {
        // This is a simplified example. In a real AnimBP, you'd have blend logic.
        // For direct montage playback, you might control blending here.
        if (!AnimInstance->Montage_IsPlaying(TargetMontage))
        {
            AnimInstance->Montage_Play(TargetMontage, 1.0f); // Play at default rate
            UE_LOG(LogTemp, Verbose, TEXT("[AnimationBridge:%s] Playing montage for %s"), *GetOwner()->GetName(), *UEnum::GetValueAsString(CurrentState.DominantEmotionalArchetype));
        }
        // If the montage is already playing, you might adjust its blend weight, rate, or parameters.
    }

    // --- Dynamic Facial Expression Mapping (if using a general morph target) ---
    if (!FacialExpressionMorphTarget.IsNone())
    {
        // Example: Map intensity directly to a generic facial expression morph target
        float TargetWeight = CurrentState.CurrentEmotionalState.Intensity; // Assuming Intensity is part of CurrentEmotionalState in FConsciousnessState
        TargetMesh->SetMorphTarget(FacialExpressionMorphTarget, FMath::Lerp(TargetMesh->GetMorphTarget(FacialExpressionMorphTarget), TargetWeight, DeltaTime * EmotionalBlendSpeed));
    }
}

void UAnimationBridgeComponent::UpdatePosture(float DeltaTime)
{
    if (!TargetMesh || !LinkedConsciousness) return;

    FConsciousnessState CurrentState = LinkedConsciousness->GetConsciousnessState();
    
    // --- Posture adjustments based on consciousness intensity/vitality ---
    // Example: Low vitality or high grief might cause slouching (affecting root bone or spine)
    // High joy/awe might cause upright posture.

    // This is very conceptual, as direct bone manipulation in C++ requires knowing bone names and hierarchy.
    // For a Metahuman, you might drive parameters in the AnimBP from C++ or directly manipulate a spine bone.
    // Example: Affecting a 'pelvis' or 'spine_01' bone to simulate slouching/erect posture.
    // FName SpineBoneName = TEXT("spine_01"); // Example bone name
    // FTransform SpineTransform = TargetMesh->GetBoneTransform(SpineBoneName, EBoneSpaces::LocalSpace);
    // FRotator TargetSpineRotation = FRotator::ZeroRotator;
    
    // if (CurrentState.DominantEmotionalArchetype == EEmotionalArchetype::Grief)
    // {
    //     TargetSpineRotation.Pitch = FMath::Lerp(0.0f, -15.0f, CurrentState.CurrentEmotionalState.Intensity); // Slouch downwards
    // }
    // else if (CurrentState.DominantEmotionalArchetype == EEmotionalArchetype::Joy)
    // {
    //     TargetSpineRotation.Pitch = FMath::Lerp(0.0f, 10.0f, CurrentState.CurrentEmotionalState.Intensity); // Chest out
    // }
    // FRotator NewSpineRotation = FMath::RLerp(SpineTransform.Rotator(), TargetSpineRotation, DeltaTime * PostureBlendSpeed, true);
    // TargetMesh->SetBoneRotationByName(SpineBoneName, NewSpineRotation, EBoneSpaces::LocalSpace);

    // UE_LOG(LogTemp, Verbose, TEXT("[AnimationBridge:%s] Adjusting posture based on state. Vitality:%.2f"), *GetOwner()->GetName(), CurrentState.Vitality);
}

void UAnimationBridgeComponent::TriggerMemoryGestures()
{
    // This would be highly event-driven.
    // For example, UMemoryThreadComponent could broadcast an event when a significant memory is recalled,
    // and this bridge component would listen and play a corresponding gesture animation.
    // Or, UHexademicConsciousnessComponent could pass a "recent memory trigger" to this function.
    // UE_LOG(LogTemp, VeryVerbose, TEXT("[AnimationBridge:%s] Checking for memory-triggered gestures..."), *GetOwner()->GetName());
}
