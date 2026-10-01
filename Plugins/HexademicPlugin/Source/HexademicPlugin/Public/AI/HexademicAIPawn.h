#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h" // Assuming most Hexademic NPCs are characters
#include "Components/HexademicConsciousnessComponent.h" // The core consciousness
#include "Bridge/BehaviorTreeBridge.h" // BT integration
#include "Bridge/PerceptionBridge.   // Perception integration
#include "Bridge/AnimationBridgeComponent.h" // Animation integration
#include "AI/EmotionalDecisionMaker.h" // Custom decision maker
#include "AI/HexademicAIPawn.generated.h"

UCLASS()
class HEXADEMICPLUGIN_API AHexademicAIPawn : public ACharacter
{
    GENERATED_BODY()

public:
    AHexademicAIPawn();

protected:
    virtual void BeginPlay() override;

public:
    // Components that define the Hexademic AI's consciousness
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    TObjectPtr<UHexademicConsciousnessComponent> HexademicConsciousnessComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    TObjectPtr<UBehaviorTreeBridge> BehaviorTreeBridgeComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    TObjectPtr<UPerceptionBridge> PerceptionBridgeComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    TObjectPtr<UAnimationBridgeComponent> AnimationBridgeComp;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    TObjectPtr<UEmotionalDecisionMaker> EmotionalDecisionMakerComp; // Custom decision maker

    // Add other necessary components here as default sub-objects, e.g.:
    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    // TObjectPtr<UEmotionCognitionComponent> EmotionMind;
    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hexademic Components")
    // TObjectPtr<UBiologicalNeedsComponent> BiologicalNeeds;
    // ... etc, for all the sub-components HexademicConsciousnessComp relies on

    /**
     * @brief Get a reference to this pawn's Hexademic Consciousness Component.
     */
    UFUNCTION(BlueprintPure, Category = "Hexademic AI")
    UHexademicConsciousnessComponent* GetConsciousnessComponent() const { return HexademicConsciousnessComp; }

protected:
    // Optional: Function to initialize component references after construction/begin play
    void SetupHexademicComponents();
};
