#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicCore.h" // For FAetherTouchPacket, FPack.edHexaSigilNode
#include "ConsciousnessBridgeComponent.generated.h"

// Forward Declarations for components this bridge communicates with
class UEmotionCognitionComponent;
class UEmbodiedAvatarComponent;
class UAvatarMotionLinkComponent;
class UDUIDSOrchestrator; // Main API Orchestrator (from future module)
class UHexademicWavefrontAPI; // Wavefront API (from future module)

UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMICBRIDGE_API UConsciousnessBridgeComponent : public UActorComponent {
    GENERATED_BODY()

public:
    UConsciousnessBridgeComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    // References to other core components, set in editor or at runtime
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEmotionCognitionComponent> EmotionMind;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UEmbodiedAvatarComponent> AvatarBody;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UAvatarMotionLinkComponent> AvatarMotion; // For linking motion to emotions

    // Reference to the main Hexademic API orchestrator (optional, for direct state querying)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UDUIDSOrchestrator> MainAPIOrchestrator;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<UHexademicWavefrontAPI> WavefrontAPI; // For fetching active sigils or gems

    /**
     * @brief Processes an incoming haptic packet, routing it to the Mind for emotional modulation
     * and to the Body for visual feedback, completing the haptic-to-emotion-to-visual pipeline.
     * @param Packet The FAetherTouchPacket containing haptic event details.
     */
    UFUNCTION(BlueprintCallable, Category = "ConsciousnessBridge")
    void ProcessIncomingHapticPacket(const FAetherTouchPacket& Packet);
    /**
     * @brief Updates the avatar's body and motion based on the current emotional state of the Mind.
     * This is the core Mind -> Body synchronization loop.
     */
    UFUNCTION(BlueprintCallable, Category = "ConsciousnessBridge")
    void UpdateEmbodiedPhenomena();
    /**
     * @brief Applies the Hexademic state (e.g., active Sigil Nodes) to the Avatar's body.
     * This integrates the higher-level consciousness state with physical manifestation.
     */
    UFUNCTION(BlueprintCallable, Category = "ConsciousnessBridge")
    void ApplyHexademicState();
protected:
    // Internal timer for UpdateEmbodiedPhenomena to control update frequency
    float UpdatePhenomenaAccumulatedTime = 0.0f;
    UPROPERTY(EditAnywhere, Category = "ConsciousnessBridge|Tuning")
    float EmbodiedPhenomenaUpdateFrequency = 30.0f; // Hz

};
