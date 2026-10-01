#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/HexademicConsciousnessComponent.h" // For UHexademicConsciousnessComponent
#include "Bridge/BehaviorTreeBridge.h" // For UBehaviorTreeBridge
#include "Bridge/PerceptionBridge.h"   // For UPerceptionBridge
#include "AI/HexademicAIController.generated.h"

// Forward Declarations
class UBehaviorTree;
class UBlackboardComponent;
class UAIPerceptionComponent;
class UEmotionalDecisionMaker; // Our custom decision maker

UCLASS()
class HEXADEMICPLUGIN_API AHexademicAIController : public AAIController
{
    GENERATED_BODY()

public:
    AHexademicAIController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

public:
    /**
     * @brief Gets the Hexademic Consciousness Component attached to the controlled pawn.
     * @return The UHexademicConsciousnessComponent, or nullptr if not found.
     */
    UFUNCTION(BlueprintPure, Category = "Hexademic AI")
    UHexademicConsciousnessComponent* GetHexademicConsciousnessComponent() const { return HexademicConsciousnessComp; }

    /**
     * @brief Sets the Behavior Tree to use for this AI.
     * @param InBehaviorTree The Behavior Tree asset.
     */
    UFUNCTION(BlueprintCallable, Category = "Hexademic AI")
    void SetBehaviorTree(UBehaviorTree* InBehaviorTree);

    // References to core Hexademic components (expected to be on the controlled pawn)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UHexademicConsciousnessComponent> HexademicConsciousnessComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UBehaviorTreeBridge> BehaviorTreeBridgeComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UPerceptionBridge> PerceptionBridgeComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UEmotionalDecisionMaker> EmotionalDecisionMakerComp; // Our custom decision maker

protected:
    // The Behavior Tree asset to run
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
    TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

    // Internal helper to initialize references and bridges
    void InitializeHexademicAI();
};
