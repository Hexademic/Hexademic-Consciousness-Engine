#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/HexademicConsciousnessComponent.h" // For UHexademicConsciousnessComponent
#include "Core/EmotionalArchetype.h" // For EEmotionalArchetype
#include "BehaviorTree/BlackboardComponent.h" // For UBlackboardComponent
#include "AI/EmotionalDecisionMaker.generated.h"

// Forward Declarations
class UBehaviorTreeComponent; // If it needs to directly influence BT execution

// FEmotionalDecision: Represents a decision triggered by a specific emotional state
USTRUCT(BlueprintType)
struct HEXADEMICPLUGIN_API FEmotionalDecision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Decision")
    EEmotionalArchetype TriggerEmotion; // The emotional archetype that triggers this decision
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Decision")
    float MinIntensityThreshold; // Minimum intensity of the emotion to trigger
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Decision")
    FString ActionTag; // A tag describing the action to take (e.g., "AttackAggressively", "FleeInPanic", "SeekComfort")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Decision")
    float Priority; // Higher priority decisions override lower ones
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Decision")
    float CooldownTime; // How long before this decision can be triggered again

    FEmotionalDecision()
        : TriggerEmotion(EEmotionalArchetype::Curiosity), MinIntensityThreshold(0.7f),
          ActionTag(TEXT("Default")), Priority(1.0f), CooldownTime(5.0f) {}
};

UCLASS(ClassGroup=(HexademicAI), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UEmotionalDecisionMaker : public UActorComponent
{
    GENERATED_BODY()

public:
    UEmotionalDecisionMaker();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    /**
     * @brief Sets the linked Hexademic Consciousness Component.
     */
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness);

    /**
     * @brief Sets the linked Blackboard Component to potentially write decisions to.
     */
    UFUNCTION(BlueprintCallable, Category = "Emotional AI")
    void SetLinkedBlackboard(UBlackboardComponent* InBlackboard);

    // Reference to the consciousness component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UHexademicConsciousnessComponent> LinkedConsciousness;
    // Reference to the Blackboard component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UBlackboardComponent> LinkedBlackboard;
    // Optional: Reference to BehaviorTreeComponent for direct control
    // UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    // TObjectPtr<UBehaviorTreeComponent> LinkedBehaviorTree;

    // List of predefined emotional decisions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Emotional Decisions")
    TArray<FEmotionalDecision> EmotionalDecisions;

    // Blackboard Key Name where the emotional action tag will be written
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Mapping")
    FName EmotionalActionKeyName = TEXT("EmotionalActionTag");

protected:
    // Internal function to evaluate current emotional state against decisions
    void EvaluateEmotionalDecisions();

    // Track cooldowns for decisions
    TMap<FString, float> DecisionCooldowns; // Key: ActionTag, Value: Time remaining
};
