#include "AI/HexademicAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "AI/EmotionalDecisionMaker.h" // Custom decision maker

AHexademicAIController::AHexademicAIController()
{
    // Initialize AI Perception Component (if not inheriting from a base that does)
    // AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
    // Set our custom BehaviorTreeComponent and BlackboardComponent if needed
    // BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    // Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void AHexademicAIController::BeginPlay()
{
    Super::BeginPlay();
    InitializeHexademicAI(); // Perform setup after components are ready
}

void AHexademicAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    // When we possess a pawn, try to get its Hexademic components
    HexademicConsciousnessComp = InPawn->FindComponentByClass<UHexademicConsciousnessComponent>();
    BehaviorTreeBridgeComp = InPawn->FindComponentByClass<UBehaviorTreeBridge>();
    PerceptionBridgeComp = InPawn->FindComponentByClass<UPerceptionBridge>();
    EmotionalDecisionMakerComp = InPawn->FindComponentByClass<UEmotionalDecisionMaker>(); // Find custom decision maker

    // If we have a Hexademic Consciousness Component, start the BT
    if (HexademicConsciousnessComp)
    {
        UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Possessed Pawn %s with Consciousness Component."), *GetName(), *InPawn->GetName());

        // Initialize and run Behavior Tree
        if (BehaviorTreeAsset)
        {
            RunBehaviorTree(BehaviorTreeAsset); // This will automatically create/use BT and Blackboard components
            UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Running Behavior Tree: %s"), *GetName(), *BehaviorTreeAsset->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[HexademicAIController:%s] No Behavior Tree Asset assigned!"), *GetName());
        }

        // Link bridges if they exist on the pawn
        if (BehaviorTreeBridgeComp && GetBlackboardComponent())
        {
            BehaviorTreeBridgeComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            BehaviorTreeBridgeComp->SetLinkedBlackboard(GetBlackboardComponent());
            UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Linked BehaviorTreeBridge."), *GetName());
        }
        if (PerceptionBridgeComp && GetAIPerceptionComponent())
        {
            PerceptionBridgeComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            PerceptionBridgeComp->SetLinkedPerception(GetAIPerceptionComponent());
            UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Linked PerceptionBridge."), *GetName());
        }
        if (EmotionalDecisionMakerComp && GetBlackboardComponent())
        {
            EmotionalDecisionMakerComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            EmotionalDecisionMakerComp->SetLinkedBlackboard(GetBlackboardComponent());
            UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Linked EmotionalDecisionMaker."), *GetName());
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[HexademicAIController:%s] Possessed Pawn %s without Hexademic Consciousness Component. AI will operate in traditional mode."), *GetName(), *InPawn->GetName());
    }
}

void AHexademicAIController::OnUnPossess()
{
    Super::OnUnPossess();
    UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Unpossessed pawn."), *GetName());
    // Clear references if needed to prevent stale pointers
    HexademicConsciousnessComp = nullptr;
    BehaviorTreeBridgeComp = nullptr;
    PerceptionBridgeComp = nullptr;
    EmotionalDecisionMakerComp = nullptr;
}

void AHexademicAIController::SetBehaviorTree(UBehaviorTree* InBehaviorTree)
{
    BehaviorTreeAsset = InBehaviorTree;
    // If already possessed, restart the BT with the new asset
    if (GetPawn() && BehaviorTreeAsset && !BehaviorTreeComponent->IsRunning())
    {
        RunBehaviorTree(BehaviorTreeAsset);
        UE_LOG(LogTemp, Log, TEXT("[HexademicAIController:%s] Switched to new Behavior Tree: %s"), *GetName(), *BehaviorTreeAsset->GetName());
    }
}

void AHexademicAIController::InitializeHexademicAI()
{
    // This function can be used for any setup logic that needs to run after all components on the AIController
    // and its possessed Pawn have been initialized. For example, setting up initial blackboard values
    // or subscribing to global consciousness events.
    if (GetBlackboardComponent() && HexademicConsciousnessComp)
    {
        // Example: Set initial dominant emotion on blackboard
        GetBlackboardComponent()->SetValueAsEnum(TEXT("DominantEmotion"), (uint8)HexademicConsciousnessComp->GetConsciousnessState().DominantEmotionalArchetype);
    }
}
