#include "Bridge/BehaviorTreeBridge.h"
#include "AIController.h" // For AAIController
#include "GameFramework/Pawn.h" // For APawn (owner of AIController)

UBehaviorTreeBridge::UBehaviorTreeBridge()
{
    PrimaryComponentTick.bCanEverTick = false; // Updates are typically driven by BT services or tasks
}

void UBehaviorTreeBridge::BeginPlay()
{
    Super::BeginPlay();

    // Attempt to find linked components if not set in editor
    if (!LinkedConsciousness)
    {
        LinkedConsciousness = GetOwner()->FindComponentByClass<UHexademicConsciousnessComponent>();
        if (!LinkedConsciousness) UE_LOG(LogTemp, Warning, TEXT("[BTBridge:%s] LinkedConsciousness not found."), *GetOwner()->GetName());
    }

    // Attempt to find Blackboard and BehaviorTree via AIController
    AAIController* AIController = Cast<AAIController>(GetOwner());
    if (!AIController)
    {
        APawn* OwnerPawn = Cast<APawn>(GetOwner());
        if (OwnerPawn)
        {
            AIController = Cast<AAIController>(OwnerPawn->GetController());
        }
    }
    
    if (AIController)
    {
        if (!LinkedBlackboard)
        {
            LinkedBlackboard = AIController->GetBlackboardComponent();
            if (!LinkedBlackboard) UE_LOG(LogTemp, Warning, TEXT("[BTBridge:%s] LinkedBlackboard not found via AIController."), *GetOwner()->GetName());
        }
        if (!LinkedBehaviorTree)
        {
            // BehaviorTreeComponent is usually managed by the AIController directly.
            // You might need a more direct way to get this if it's not exposed, or query AAIController.
            // For now, assume it's directly accessible or can be assigned.
            // LinkedBehaviorTree = AIController->GetBehaviorTreeComponent(); // No direct getter for this
            UE_LOG(LogTemp, Warning, TEXT("[BTBridge:%s] LinkedBehaviorTree is typically managed by AIController; set manually if needed."), *GetOwner()->GetName());
        }
    }
}

void UBehaviorTreeBridge::SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness)
{
    LinkedConsciousness = InConsciousness;
}

void UBehaviorTreeBridge::SetLinkedBlackboard(UBlackboardComponent* InBlackboard)
{
    LinkedBlackboard = InBlackboard;
}

void UBehaviorTreeBridge::SetLinkedBehaviorTree(UBehaviorTreeComponent* InBehaviorTree)
{
    LinkedBehaviorTree = InBehaviorTree;
}

void UBehaviorTreeBridge::UpdateBlackboardFromConsciousness()
{
    if (!LinkedConsciousness || !LinkedBlackboard)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTBridge:%s] Cannot update Blackboard: Missing consciousness or blackboard."), *GetOwner()->GetName());
        return;
    }

    FConsciousnessState State = LinkedConsciousness->GetConsciousnessState();

    // Update Blackboard keys
    if (LinkedBlackboard->DoesKeyExist(DominantEmotionKeyName))
    {
        LinkedBlackboard->SetValueAsEnum(DominantEmotionKeyName, (uint8)State.DominantEmotionalArchetype);
    }
    if (LinkedBlackboard->DoesKeyExist(VitalityKeyName))
    {
        LinkedBlackboard->SetValueAsFloat(VitalityKeyName, State.Vitality);
    }
    if (LinkedBlackboard->DoesKeyExist(CognitiveLoadKeyName))
    {
        LinkedBlackboard->SetValueAsFloat(CognitiveLoadKeyName, State.CognitiveLoad);
    }
    if (LinkedBlackboard->DoesKeyExist(CoherenceKeyName))
    {
        LinkedBlackboard->SetValueAsFloat(CoherenceKeyName, State.LatticeSnapshot.Amplitude); // Using Lattice Amplitude as proxy for Coherence for now
    }

    UE_LOG(LogTemp, Verbose, TEXT("[BTBridge:%s] Blackboard updated from consciousness. Vitality: %.2f, Emotion: %s"),
        *GetOwner()->GetName(), State.Vitality, *UEnum::GetValueAsString(State.DominantEmotionalArchetype));
}

EEmotionalArchetype UBehaviorTreeBridge::GetDominantEmotionalArchetype() const
{
    if (LinkedConsciousness)
    {
        return LinkedConsciousness->GetConsciousnessState().DominantEmotionalArchetype;
    }
    return EEmotionalArchetype::Curiosity; // Default or neutral
}

float UBehaviorTreeBridge::GetVitality() const
{
    if (LinkedConsciousness)
    {
        return LinkedConsciousness->GetConsciousnessState().Vitality;
    }
    return 0.0f;
}

float UBehaviorTreeBridge::GetCognitiveLoad() const
{
    if (LinkedConsciousness)
    {
        return LinkedConsciousness->GetConsciousnessState().CognitiveLoad;
    }
    return 0.0f;
}

bool UBehaviorTreeBridge::IsCoherenceHigh(float Threshold) const
{
    if (LinkedConsciousness)
    {
        // Using HexLattice Amplitude as a proxy for coherence in this context
        return LinkedConsciousness->GetConsciousnessState().LatticeSnapshot.Amplitude >= Threshold;
    }
    return false;
}
