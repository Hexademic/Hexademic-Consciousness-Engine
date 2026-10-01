#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraComponent.h"
#include "RenderGraphResources.h" // For TRefCountPtr, FRDGPooledBuffer, FPooledRenderTarget
#include "RenderGraphBuilder.h" // For FRDGBuilder
#include "GlobalShader.h" // For FGlobalShader, SHADER_PARAMETER_STRUCT
#include "ShaderParameterStruct.h" // For BEGIN_SHADER_PARAMETER_STRUCT
#include "HexademicCore.h" // For FAetherTouchPacket
#include "EmbodiedAvatarComponent.generated.h"

// Forward Declarations for other Unreal Engine classes
class USkeletalMeshComponent;
class UNiagaraSystem;

// FWavefrontSkinState: Optimized skin state for parallel processing on the GPU.
// Packs emotional pulse and breath pulse rate into a single uint32 for efficiency.
USTRUCT(BlueprintType)
struct HEXADEMICBODY_API FWavefrontSkinState
{
    GENERATED_BODY()

    // Packed emotional data for GPU processing
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    uint32 EmotionalStatePack; // EmotionalPulse (16-bit) | PulseRate (16-bit)

    // Performance optimized accessors for packed data
    FORCEINLINE float GetEmotionalPulse() const
    {
        return float(EmotionalStatePack & 0xFFFF) / 65535.0f;
    }

    FORCEINLINE void SetEmotionalPulse(float Pulse)
    {
        uint32 PulseInt = FMath::Clamp(uint32(Pulse * 65535.0f), 0u, 65535u);
        EmotionalStatePack = (EmotionalStatePack & 0xFFFF0000) | PulseInt;
    }

    FORCEINLINE float GetPulseRate() const
    {
        return float((EmotionalStatePack >> 16) & 0xFFFF) / 65535.0f;
    }

    FORCEINLINE void SetPulseRate(float Rate)
    {
        uint32 RateInt = FMath::Clamp(uint32(Rate * 65535.0f), 0u, 65535u);
        EmotionalStatePack = (EmotionalStatePack & 0x0000FFFF) | (RateInt << 16);
    }
};

// UEmbodiedAvatarComponent: Manages the avatar's physical manifestation and visual responses.
UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMICBODY_API UEmbodiedAvatarComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEmbodiedAvatarComponent();
    // Wavefront processing configuration for skin updates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wavefront Processing")
    bool bEnableWavefrontSkinProcessing = true;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wavefront Processing")
    float SkinUpdateFrequency = 60.0f; // Hz for GPU skin updates

    // Target mesh and materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Avatar")
    TArray<UMaterialInstanceDynamic*> DynamicMaterials; // Materials applied to the target mesh

    // Niagara systems for breathing and other visual effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TObjectPtr<UNiagaraSystem> BreathSystemTemplate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effects")
    TObjectPtr<UNiagaraComponent> BreathNiagara;
    // Current skin state, updated and used by wavefront processing
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wavefront State")
    FWavefrontSkinState CurrentSkinState;
    // Map for haptic region to skeletal bone mapping for visual effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Mapping")
    TMap<FString, FName> RegionToBoneMap;
    // Reference to a particle system for haptic pulses
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic Effects")
    TObjectPtr<UParticleSystem> HapticPulseParticleSystem;
    // Reference to the main skin material dynamic instance, if there's a master material for the whole body
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Avatar")
    TObjectPtr<UMaterialInstanceDynamic> SkinMaterial;
    // For storing the sigil glow color if it's needed in the component's state
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wavefront State")
    FLinearColor CurrentSigilGlowColor;

    // Component lifecycle functions
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    /**
     * @brief Updates the avatar's skin state based on an emotional pulse, leveraging wavefront processing.
     * @param EmotionalPulse A normalized value [0.0, 1.0] representing emotional intensity, driving skin visual effects.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront Embodiment")
    void UpdateSkinStateWavefront(float EmotionalPulse);
    /**
     * @brief Synchronizes the avatar's Niagara-based breathing effect with a given pulse rate.
     * @param PulseRate The rate at which the breathing effect should pulse.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront Embodiment")
    void SyncWithNiagaraBreathWavefront(float PulseRate);
    /**
     * @brief Applies a global sigil color effect to the avatar's skin materials.
     * @param SigilID The ID of the sigil.
     * @param Color The linear color to apply for the sigil glow.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront Embodiment")
    void ApplySigilColorWavefront(const FString& SigilID, const FLinearColor& Color);
    /**
     * @brief Attaches this component to a Metahuman (or any) skeletal mesh, initializing its materials.
     * @param InMesh The skeletal mesh component to attach to.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront Embodiment")
    void AttachToMetahumanSkeleton(USkeletalMeshComponent* InMesh);
    /**
     * @brief Processes incoming haptic feedback, triggering visual responses on the avatar.
     * This uses the RegionToBoneMap to localize effects.
     * @param Packet The FAetherTouchPacket containing haptic event details.
     */
    UFUNCTION(BlueprintCallable, Category = "Avatar|Haptics")
    void ReceiveHapticFeedback(const FAetherTouchPacket& Packet);
    /**
     * @brief Dispatches the GPU skin update pass, pushing current skin state to the shader.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront Embodiment")
    void ProcessSkinWavefrontBatch();

private:
    // GPU resources for wavefront skin processing
    TRefCountPtr<FRDGPooledBuffer> SkinStateBuffer;
    TRefCountPtr<FPooledRenderTarget> SkinResponseTexture; // Output texture from the skin compute shader

    // Internal methods for GPU resource management and execution
    void InitializeWavefrontResources();
    void ReleaseWavefrontResources();
    void ExecuteSkinWavefrontPass(class FRDGBuilder& GraphBuilder); // Needs FRDGBuilder
    void UpdateMaterialParametersBatch(); // Applies results from GPU (or CPU) to materials
    void UpdatePerformanceMetrics(float DeltaTime); // Tracks processing time
    TCircularBuffer<float> ProcessingTimeHistory; // History for averaging
    float AverageSkinProcessingTime = 0.0f; // Average time for one GPU pass

    // New function: UpdateMaterialParametersBatch_RenderThread
    // This function will be called on the Render Thread to update material parameters.
    void UpdateMaterialParametersBatch_RenderThread(FLinearColor GPUSkinResponse);

};
