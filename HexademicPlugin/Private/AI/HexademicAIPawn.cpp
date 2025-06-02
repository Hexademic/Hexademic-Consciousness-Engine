#include "AI/HexademicAIPawn.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h" // If this pawn is also a stimuli source

AHexademicAIPawn::AHexademicAIPawn()
{
    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // --- Initialize Hexademic Components as default sub-objects ---
    // These components are added here directly so they are present on any Blueprint derived from AHexademicAIPawn.
    HexademicConsciousnessComp = CreateDefaultSubobject<UHexademicConsciousnessComponent>(TEXT("HexademicConsciousness"));
    BehaviorTreeBridgeComp = CreateDefaultSubobject<UBehaviorTreeBridge>(TEXT("BehaviorTreeBridge"));
    PerceptionBridgeComp = CreateDefaultSubobject<UPerceptionBridge>(TEXT("PerceptionBridge"));
    AnimationBridgeComp = CreateDefaultSubobject<UAnimationBridgeComponent>(TEXT("AnimationBridge"));
    EmotionalDecisionMakerComp = CreateDefaultSubobject<UEmotionalDecisionMaker>(TEXT("EmotionalDecisionMaker")); // Custom decision maker

    // Add other sub-components that HexademicConsciousnessComp is expected to own/manage
    // This list should ideally match the UPROPERTY references in UHexademicConsciousnessComponent
    // if you want them created as default sub-objects on the pawn itself.
    // Alternatively, UHexademicConsciousnessComponent can create them dynamically or auto-discover them.
    // For a cleaner approach, HexademicConsciousnessComp should be responsible for creating its *own* sub-components.
    // The current setup means these are all *siblings* on the Pawn, and HexademicConsciousnessComp gets refs.
    // If you prefer HexademicConsciousnessComp to own them, then these 'CreateDefaultSubobject' calls should be *inside*
    // UHexademicConsciousnessComponent's constructor or BeginPlay, and simply be UPROPERTY references here.

    // Example of adding other sub-components directly to the Pawn if they are not managed by HexademicConsciousnessComp
    // EmotionMind = CreateDefaultSubobject<UEmotionCognitionComponent>(TEXT("EmotionMind"));
    // BiologicalNeeds = CreateDefaultSubobject<UBiologicalNeedsComponent>(TEXT("BiologicalNeeds"));
    // ... etc.

    // Configure standard character properties (inherited from ACharacter)
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Set up a stimuli source component if this pawn is intended to be sensed by others
    // UAIPerceptionStimuliSourceComponent* StimuliSourceComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
}

void AHexademicAIPawn::BeginPlay()
{
    Super::BeginPlay();
    SetupHexademicComponents(); // Call to link up components
}

void AHexademicAIPawn::SetupHexademicComponents()
{
    // Link the AI components to the HexademicConsciousnessComponent if it exists
    if (HexademicConsciousnessComp)
    {
        // Pass references to the bridges
        if (BehaviorTreeBridgeComp)
        {
            BehaviorTreeBridgeComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            // Blackboard and BehaviorTree components are typically set by the AIController during Possess.
            // This is just a good place to ensure the bridge has the consciousness reference.
        }
        if (PerceptionBridgeComp)
        {
            PerceptionBridgeComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            // PerceptionComponent is usually set by the AIController.
        }
        if (AnimationBridgeComp)
        {
            AnimationBridgeComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            AnimationBridgeComp->SetTargetMesh(GetMesh()); // Set the skeletal mesh
        }
        if (EmotionalDecisionMakerComp)
        {
            EmotionalDecisionMakerComp->SetLinkedConsciousness(HexademicConsciousnessComp);
            // Blackboard is set by the AIController
        }
    }
}
