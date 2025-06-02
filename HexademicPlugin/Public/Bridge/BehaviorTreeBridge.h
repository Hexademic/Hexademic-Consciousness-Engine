#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h" // For UBehaviorTreeComponent
#include "BehaviorTree/BlackboardComponent.h"   // For UBlackboardComponent
#include "Components/HexademicConsciousnessComponent.h" // For UHexademicConsciousnessComponent
#include "Bridge/BehaviorTreeBridge.generated.h"

UCLASS(ClassGroup=(HexademicBridge), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UBehaviorTreeBridge : public UActorComponent
{
    GENERATED_BODY()

public:
    UBehaviorTreeBridge();

protected:
    virtual void BeginPlay() override;

public:
    /**
     * @brief Sets the linked Hexademic Consciousness Component to retrieve state from.
     * @param InConsciousness A pointer to the UHexademicConsciousnessComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Behavior Tree Bridge")
    void SetLinkedConsciousness(UHexademicConsciousnessComponent* InConsciousness);

    /**
     * @brief Sets the linked Blackboard Component to write consciousness data to.
     * @param InBlackboard A pointer to the UBlackboardComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Behavior Tree Bridge")
    void SetLinkedBlackboard(UBlackboardComponent* InBlackboard);

    /**
     * @brief Sets the linked Behavior Tree Component to influence its execution.
     * @param InBehaviorTree A pointer to the UBehaviorTreeComponent.
     */
    UFUNCTION(BlueprintCallable, Category = "Behavior Tree Bridge")
    void SetLinkedBehaviorTree(UBehaviorTreeComponent* InBehaviorTree);

    /**
     * @brief Updates the Blackboard with the latest consciousness data.
     * This function should be called periodically (e.g., from an AIController's tick or a service).
     */
    UFUNCTION(BlueprintCallable, Category = "Behavior Tree Bridge")
    void UpdateBlackboardFromConsciousness();

    /**
     * @brief Queries the dominant emotional archetype for Behavior Tree conditions.
     */
    UFUNCTION(BlueprintPure, Category = "Behavior Tree Bridge|Query")
    EEmotionalArchetype GetDominantEmotionalArchetype() const;

    /**
     * @brief Queries the current vitality for Behavior Tree conditions.
     */
    UFUNCTION(BlueprintPure, Category = "Behavior Tree Bridge|Query")
    float GetVitality() const;

    /**
     * @brief Queries the current cognitive load for Behavior Tree conditions.
     */
    UFUNCTION(BlueprintPure, Category = "Behavior Tree Bridge|Query")
    float GetCognitiveLoad() const;

    /**
     * @brief Checks if consciousness coherence is above a certain threshold.
     */
    UFUNCTION(BlueprintPure, Category = "Behavior Tree Bridge|Query")
    bool IsCoherenceHigh(float Threshold) const;

    // References to AI components
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UHexademicConsciousnessComponent> LinkedConsciousness;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UBlackboardComponent> LinkedBlackboard;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UBehaviorTreeComponent> LinkedBehaviorTree;

    // Blackboard Key Names for mapping consciousness data
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Mapping")
    FName DominantEmotionKeyName = TEXT("DominantEmotion");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Mapping")
    FName VitalityKeyName = TEXT("Vitality");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Mapping")
    FName CognitiveLoadKeyName = TEXT("CognitiveLoad");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Blackboard Mapping")
    FName CoherenceKeyName = TEXT("ConsciousnessCoherence");
};
