#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RenderGraphBuilder.h"
#include "GlobalShader.h"
#include "ShaderParameterStruct.h"
#include "HexademicCore.h" // For FPackedHexaSigilNode, FHexademicGem
#include "HexademicWavefrontAPI.generated.h"

// Forward Declarations for Shader classes (if needed as distinct global shaders)
class FWavefrontSigilComputeShader;
class FHexademicGemSynthesisShader;
/**
 * @brief Manages GPU-accelerated processing of Hexademic consciousness elements (Sigils)
 * and the synthesis of high-coherence cognitive artifacts (Gems).
 * This component acts as a central hub for GPU-based consciousness operations.
 */
UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMICAPI_API UHexademicWavefrontAPI : public UActorComponent
{
    GENERATED_BODY()

public:
    UHexademicWavefrontAPI();
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    // Configuration for wavefront processing
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wavefront Processing")
    int32 MaxSigilNodes = 1024; // Maximum number of active sigil nodes on GPU
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wavefront Processing")
    float WavefrontUpdateRate = 30.0f; // Hz for GPU wavefront updates
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wavefront Processing")
    float GemSynthesisFrequency = 5.0f; // Hz for attempting to synthesize new gems

    // Array of currently active Sigil Nodes
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wavefront State")
    TArray<FPackedHexaSigilNode> ActiveSigilNodes;
    // Array of synthesized Hexademic Gems
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wavefront State")
    TArray<FHexademicGem> SynthesizedGems;
    /**
     * @brief Adds a new sigil node to the active processing queue.
     * @param NewSigil The FPackedHexaSigilNode to add.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront API")
    void AddSigilNode(const FPackedHexaSigilNode& NewSigil);
    /**
     * @brief Removes a sigil node by its ID from the active processing queue.
     * @param SigilID The ID of the sigil to remove.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront API")
    void RemoveSigilNode(const FString& SigilID);
    /**
     * @brief Initiates the GPU wavefront processing for all active sigil nodes.
     * This will run compute shaders for coherence, resonance, and interaction.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront API")
    void ProcessWavefrontGPU();
    /**
     * @brief Attempts to synthesize new FHexademicGem artifacts based on current sigil patterns.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront API")
    void SynthesizeHexademicGems();
    /**
     * @brief Initializes GPU resources needed for wavefront processing.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront API")
    void InitializeWavefrontProcessing();
    /**
     * @brief Cleans up GPU resources used by wavefront processing.
     */
    UFUNCTION(BlueprintCallable, Category = "Wavefront API")
    void ShutdownWavefrontProcessing();
    // Getter for active sigil nodes (primarily for UDUIDSOrchestrator)
    const TArray<FPackedHexaSigilNode>& GetActiveSigilNodes() const { return ActiveSigilNodes; }
    // Getter for synthesized gems (primarily for UDUIDSOrchestrator)
    const TArray<FHexademicGem>& GetSynthesizedGems() const { return SynthesizedGems; }

protected:
    // GPU resources
    TRefCountPtr<FRDGPooledBuffer> SigilNodesBuffer; // Stores FPackedHexaSigilNode data on GPU
    TRefCountPtr<FPooledRenderTarget> WavefrontOutputTexture; // Output from wavefront compute shader

    // Internal counters for update frequencies
    float AccumulatedWavefrontTime = 0.0f;
    float AccumulatedGemSynthesisTime = 0.0f;

    // Internal helper for GPU execution
    void ExecuteWavefrontComputePass(FRDGBuilder& GraphBuilder);
    void ExecuteGemSynthesisComputePass(FRDGBuilder& GraphBuilder);
    // Dummy compute shader parameters for compilation. Real shaders would be in .usf files.
    BEGIN_SHADER_PARAMETER_STRUCT(FWavefrontSigilParameters, )
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FPackedHexaSigilNode>, SigilNodesInput)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FPackedHexaSigilNode>, SigilNodesOutput) // To update sigils in place
        SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, WavefrontOutput)
        SHADER_PARAMETER(int32, NumSigilNodes)
        SHADER_PARAMETER(float, DeltaTime)
        // Add other parameters like GlobalEmotionalBias, Time, etc.
    END_SHADER_PARAMETER_STRUCT()

    // Dummy Compute Shader for Sigil Processing
    class FWavefrontSigilComputeShader : public FGlobalShader
    {
        DECLARE_GLOBAL_SHADER(FWavefrontSigilComputeShader);
        SHADER_USE_PARAMETER_STRUCT(FWavefrontSigilComputeShader, FGlobalShader);
        static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5); }
        static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) { FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment); OutEnvironment.SetDefine(TEXT("THREAD_GROUP_SIZE"), 64); }
    };

    BEGIN_SHADER_PARAMETER_STRUCT(FHexademicGemSynthesisParameters, )
        SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FPackedHexaSigilNode>, SigilNodesInput)
        SHADER_PARAMETER_RDG_BUFFER_UAV(RWStructuredBuffer<FHexademicGem>, SynthesizedGemsOutput)
        SHADER_PARAMETER(int32, NumSigilNodes)
        SHADER_PARAMETER(int32, MaxGems)
        // Add parameters like CoherenceThreshold, RandomSeed etc.
    END_SHADER_PARAMETER_STRUCT()

    // Dummy Compute Shader for Gem Synthesis
    class FHexademicGemSynthesisShader : public FGlobalShader
    {
        DECLARE_GLOBAL_SHADER(FHexademicGemSynthesisShader);
        SHADER_USE_PARAMETER_STRUCT(FHexademicGemSynthesisShader, FGlobalShader);
        static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters) { return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5); }
        static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment) { FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment); OutEnvironment.SetDefine(TEXT("THREAD_GROUP_SIZE"), 64); }
    };
};
