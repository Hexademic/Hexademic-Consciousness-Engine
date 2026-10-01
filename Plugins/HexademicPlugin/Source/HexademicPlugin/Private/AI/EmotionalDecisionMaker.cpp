#include "AI/EmotionalDecisionMaker.h"
#include "BehaviorTree/BehaviorTreeComponent.h" // If using LinkedBehaviorTree
#include "GameFramework/Character.h" // For getting the owning character if needed

UEmotionalDecisionMaker::UEmotionalDecisionMaker()
{
    PrimaryComponentTick.bCanEverTick = true;
    // Add some default emotional decisions for demonstration
    EmotionalDecisions.Add(FEmotionalDecision{EEmotionalArchetype::Rage, 0.8f, TEXT("ChargeEnemy"), 10.0f, 5.0f});
    EmotionalDecisions.Add(FEmotionalDecision{EEmotionalArchetype::Fear, 0.7f, TEXT("FleeFromThreat"), 8.0f, 3.0f});
    EmotionalDecisions.Add(FEmotionalDecision{EEmotionalArchetype::Joy, 0.9f, TEXT("ExpressHappiness"), 2.0f, 10.0f});
    EmotionalDecisions.Add(FEmotionalDecision{EEmotionalArchetype::Grief, 0.8f, TEXT("SeekComfort"), 5.0f, 7.0f});
}

void UEmotionalDecisionMaker::BeginPlay()
{
    Super::BeginPlay();
    // Auto-discover LinkedConsciousness and LinkedBlackboard if not set in editor
    if (!LinkedConsciousness)
    {
        LinkedConsciousness = GetOwner()->FindComponentByClass<UHexademicConsciousnessComponent>();
        if (!LinkedConsciousness) UE_LOG(LogTemp, Warning, TEXT("[EmotionalDecisionMaker:%s] LinkedConsciousness not found."), *GetOwner()->GetName());
    }

    if (!LinkedBlackboard)
    {
        // Try to get Blackboard Component from owner's AIController
        AAIController* AIController = Cast<AAIController>(GetOwner()->GetInstigatorController());
        if (AIController)
        {
            LinkedBlackboard = AIController->GetBlackboardComponent();
        }
        if (!LinkedBlackboard) UE_LOG(LogTemp, Warning, TEXT("[EmotionalDecisionMaker:%s] LinkedBlackboard not found."), *GetOwner()->GetName());
    }
}

void UEmotionalDecisionMaker::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Update cooldowns
    TArray<FString> CooldownsToRemove;
    for (auto& Pair : DecisionCooldowns)
    {
        Pair.Value -= DeltaTime;
        if (Pair.Value <= 0.0f)
        {
            CooldownsToRemove.Add(Pair.Key);
        }
    }
    for (const FString& Tag : CooldownsToRemove)
    {
        DecisionCooldowns.Remove(Tag);
    }

    EvaluateEmotionalDecisions();
}

void UEmotionalDecisionMaker::SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness)
{
    LinkedConsciousness = InConsciousness;
}

void UEmotionalDecisionMaker::SetLinkedBlackboard(UBlackboardComponent* InBlackboard)
{
    LinkedBlackboard = InBlackboard;
}

void UEmotionalDecisionMaker::EvaluateEmotionalDecisions()
{
    if (!LinkedConsciousness || !LinkedBlackboard) return;

    FConsciousnessState CurrentState = LinkedConsciousness->GetConsciousnessState();
    FString BestActionTag = TEXT("");
    float HighestPriority = -1.0f;

    for (const FEmotionalDecision& Decision : EmotionalDecisions)
    {
        // Check cooldown
        if (DecisionCooldowns.Contains(Decision.ActionTag) && DecisionCooldowns[Decision.ActionTag] > 0.0f)
        {
            continue; // Decision is on cooldown
        }

        // Check if the current emotional state matches the trigger emotion and intensity
        if (CurrentState.DominantEmotionalArchetype == Decision.TriggerEmotion &&
            CurrentState.CurrentEmotionalState.Intensity >= Decision.MinIntensityThreshold)
        {
            if (Decision.Priority > HighestPriority)
            {
                HighestPriority = Decision.Priority;
                BestActionTag = Decision.ActionTag;
            }
        }
    }

    if (!BestActionTag.IsEmpty())
    {
        // Write the chosen emotional action to the Blackboard
        if (LinkedBlackboard->DoesKeyExist(EmotionalActionKeyName))
        {
            LinkedBlackboard->SetValueAsString(EmotionalActionKeyName, BestActionTag);
            UE_LOG(LogTemp, Log, TEXT("[EmotionalDecisionMaker:%s] Decided on emotional action: %s (Priority: %.1f)"), *GetOwner()->GetName(), *BestActionTag, HighestPriority);
            DecisionCooldowns.Add(BestActionTag, Decision.CooldownTime); // Put on cooldown
            // Optional: If you have a LinkedBehaviorTree, you could even abort current tasks and force a new one
            // LinkedBehaviorTree->AbortCurrentTask();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[EmotionalDecisionMaker:%s] Blackboard key '%s' does not exist! Cannot write emotional action."), *GetOwner()->GetName(), *EmotionalActionKeyName.ToString());
        }
    }
    else
    {
        // If no emotional decision is triggered, clear the action tag from blackboard (or set to "None")
        if (LinkedBlackboard->DoesKeyExist(EmotionalActionKeyName))
        {
            LinkedBlackboard->ClearValue(EmotionalActionKeyName);
        }
    }
}
