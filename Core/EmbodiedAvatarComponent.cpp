#include "EmbodiedAvatarComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h" // For SpawnEmitterAtLocation
#include "Particles/ParticleSystem.h" // For UParticleSystem
#include "RenderGraphUtils.h" // For FComputeShaderUtils, CreateBuffer, QueueBufferUpload, etc.
#include "RHICommandList.h" // Required for ENQUEUE_RENDER_COMMAND
#include "Engine/Engine.h" // For GEngine->CreatePooledRenderBuffer/Target
#include "Shader.h" // For GetGlobalShaderMap
#include "GlobalShader.h" // For GET_GLOBAL_SHADER_MAP
#include "HexademicCore.h" // Ensure FAetherTouchPacket is defined
#include "RHIDefinitions.h" // For ERHIAccess

// FWavefrontSkinParameters: Shader parameters for skin processing.
BEGIN_SHADER_PARAMETER_STRUCT(FWavefrontSkinParameters, )
    SHADER_PARAMETER_RDG_BUFFER_SRV(StructuredBuffer<FWavefrontSkinState>, SkinStateBuffer) // Read-only input
    SHADER_PARAMETER_RDG_TEXTURE_UAV(RWTexture2D<float4>, SkinResponseTexture) // Output texture for skin effects
    SHADER_PARAMETER(float, EmotionalIntensity) // Direct pass-through for the shader
    SHADER_PARAMETER(float, SigilGlowStrength) // Direct pass-through for the shader
    SHADER_PARAMETER(FVector4f, SkinBaseColor) // Direct pass-through for the shader
    SHADER_PARAMETER(FVector4f, SigilGlowColor) // Direct pass-through for the shader
END_SHADER_PARAMETER_STRUCT()

// Dummy Compute Shader for Skin Processing (for compilation purposes)
// In a real project, this would be defined in a .usf file and compiled.
// For now, it's a minimal stub to allow compilation of the C++ code.
class FWavefrontSkinComputeShader : public FGlobalShader
{
    DECLARE_GLOBAL_SHADER(FWavefrontSkinComputeShader);
    SHADER_USE_PARAMETER_STRUCT(FWavefrontSkinComputeShader, FGlobalShader);

    using FParameters = FWavefrontSkinParameters;
    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }

    static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
    {
        FGlobalShader::ModifyCompilationEnvironment(Parameters, OutEnvironment);
        OutEnvironment.SetDefine(TEXT("THREAD_GROUP_SIZE"), 1); // Simple 1x1 dispatch for a single skin state
    }
};

// This line is crucial for the shader to be found and compiled by Unreal.
// You need to ensure the path points to your .usf file relative to the project's shader directory.
// The name "ProcessWavefrontSkin" must match the main function in your .usf file.
IMPLEMENT_GLOBAL_SHADER(FWavefrontSkinComputeShader, "/HexademicPlugin/FWavefrontSkinComputeShader.usf", "ProcessWavefrontSkin", SF_Compute);

UEmbodiedAvatarComponent::UEmbodiedAvatarComponent()
{
    PrimaryComponentTick.bCanEverTick = true; // Enable ticking for updates
}

void UEmbodiedAvatarComponent::BeginPlay()
{
    Super::BeginPlay();
    // Initialize dynamic materials if a TargetMesh is already set in editor
    if (TargetMesh)
    {
        for (int32 i = 0; i < TargetMesh->GetNumMaterials(); ++i)
        {
            UMaterialInterface* Mat = TargetMesh->GetMaterial(i);
            if (Mat)
            {
                UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
                TargetMesh->SetMaterial(i, DynMat);
                DynamicMaterials.Add(DynMat);
            }
        }
        // If a master SkinMaterial is set, initialize it here or ensure it's a dynamic instance
        if(SkinMaterial && DynamicMaterials.Num() > 0)
        {
            // Assuming SkinMaterial is meant to be the primary material to drive
            // For a single avatar, this would often be the first material or a specific one.
            // If it's a general material, make it dynamic if not already.
            if (!SkinMaterial->IsA(UMaterialInstanceDynamic::StaticClass()))
            {
                SkinMaterial = UMaterialInstanceDynamic::Create(SkinMaterial, this);
            }
            // If all materials are instances of SkinMaterial, they would be handled by DynamicMaterials loop.
            // If SkinMaterial is a single master instance for the whole body, apply it or link it.
        }
    }
    
    // Spawn and attach the Niagara breath system if a template is provided
    if (BreathSystemTemplate)
    {
        BreathNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(
            BreathSystemTemplate, 
            GetOwner()->GetRootComponent(), // Attach to owner's root component
            NAME_None, 
            FVector::ZeroVector, 
            FRotator::ZeroRotator, 
            EAttachLocation::SnapToTarget, 
            true // Auto-destroy
        );
    }

    InitializeWavefrontResources(); // Setup GPU buffers for wavefront processing
}
void UEmbodiedAvatarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Periodically dispatch skin wavefront processing to the GPU
    if (bEnableWavefrontSkinProcessing && SkinUpdateFrequency > 0.0f)
    {
        static float AccumulatedSkinTime = 0.0f;
        AccumulatedSkinTime += DeltaTime;
        if (AccumulatedSkinTime >= (1.0f / SkinUpdateFrequency))
        {
            ProcessSkinWavefrontBatch(); // Trigger the GPU update
            AccumulatedSkinTime = 0.0f;
        }
    }
    UpdatePerformanceMetrics(DeltaTime); // Track component performance
}

void UEmbodiedAvatarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ReleaseWavefrontResources(); // Clean up GPU resources
    Super::EndPlay(EndPlayReason);
}

// InitializeWavefrontResources: Sets up the pooled render buffers for GPU communication.
void UEmbodiedAvatarComponent::InitializeWavefrontResources()
{
    // Create a structured buffer for the skin state (usually one state per avatar)
    SkinStateBuffer = GEngine->CreatePooledRenderBuffer(
        FRDGBufferDesc::CreateStructuredDesc(sizeof(FWavefrontSkinState), 1), // Max 1 skin state for a single avatar
        TEXT("WavefrontSkinStateBuffer"),
        ERHIAccess::SRVCompute | ERHIAccess::UAVCompute // Accessible as Shader Resource View and Unordered Access View
    );
    // Create a render target for the skin response output (e.g., a single pixel for overall response)
    SkinResponseTexture = GEngine->CreatePooledRenderTarget(
        FRenderTargetDesc(1, 1, PF_R16G16B16A16_SFLOAT).SetClear(FClearValue::Transparent), // Single pixel for overall skin response
        TEXT("SkinResponseTexture")
    );
}

// ReleaseWavefrontResources: Releases the pooled render resources.
void UEmbodiedAvatarComponent::ReleaseWavefrontResources()
{
    SkinStateBuffer.SafeRelease(); // Release the skin state buffer
    SkinResponseTexture.SafeRelease(); // Release the skin response texture
    if (BreathNiagara)
    {
        BreathNiagara->DestroyComponent(); // Destroy the Niagara component if it exists
        BreathNiagara = nullptr;
    }
}

// UpdateSkinStateWavefront: Updates the CPU-side skin state to be uploaded to the GPU.
void UEmbodiedAvatarComponent::UpdateSkinStateWavefront(float EmotionalPulse)
{
    // Update the CurrentSkinState, which will be uploaded to the GPU during ProcessSkinWavefrontBatch.
    CurrentSkinState.SetEmotionalPulse(EmotionalPulse);
    // Emotional tint, subsurface strength, etc., would be calculated in the shader or material.
}

// SyncWithNiagaraBreathWavefront: Updates the Niagara breathing effect with the provided pulse rate.
void UEmbodiedAvatarComponent::SyncWithNiagaraBreathWavefront(float PulseRate)
{
    CurrentSkinState.SetPulseRate(PulseRate); // Update the CPU-side skin state with pulse rate
    if (BreathNiagara)
    {
        BreathNiagara->SetFloatParameter(FName("PulseRate"), PulseRate); // Set the Niagara parameter
    }
}

// ApplySigilColorWavefront: Applies a global sigil color to the avatar's dynamic skin materials.
void UEmbodiedAvatarComponent::ApplySigilColorWavefront(const FString& SigilID, const FLinearColor& Color)
{
    // This is a global effect. For a per-pixel sigil glow, it would be handled in the shader.
    // For now, we'll store this in a member variable if it's needed for the GPU pass,
    // or apply it directly to materials if it's a CPU-driven update.
    // Assuming SigilGlowColor is a member of this class that gets passed to the shader.
    // For simplicity, let's assume the SigilGlowColor for the shader is derived from this.
    // If you need to store this in CurrentSkinState, you'd need to expand FWavefrontSkinState.
    // For this example, we'll pass it directly to the shader parameters.
    CurrentSigilGlowColor = Color;

    for (UMaterialInstanceDynamic* Mat : DynamicMaterials)
    {
        if (Mat)
        {
            Mat->SetVectorParameterValue(FName("SigilGlowColor"), Color); // Set the SigilGlowColor parameter
            // Additional parameters might be set here based on SigilID if needed.
        }
    }
}

// AttachToMetahumanSkeleton: Attaches the component to a skeletal mesh and re-initializes dynamic materials.
void UEmbodiedAvatarComponent::AttachToMetahumanSkeleton(USkeletalMeshComponent* InMesh)
{
    TargetMesh = InMesh; // Set the target mesh
    // Re-initialize dynamic materials if attaching at runtime or if TargetMesh changes
    if (TargetMesh)
    {
        DynamicMaterials.Empty(); // Clear existing dynamic materials
        for (int32 i = 0; i < TargetMesh->GetNumMaterials(); ++i)
        {
            UMaterialInterface* Mat = TargetMesh->GetMaterial(i);
            if (Mat)
            {
                UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(Mat, this);
                TargetMesh->SetMaterial(i, DynMat); // Assign the new dynamic material
                DynamicMaterials.Add(DynMat); // Add to the list
            }
        }
    }
}
// ReceiveHapticFeedback: Processes incoming haptic packets to trigger visual responses.
void UEmbodiedAvatarComponent::ReceiveHapticFeedback(const FAetherTouchPacket& Packet)
{
    // Determine the bone to affect based on the RegionTag from the haptic packet.
    FName BoneName = RegionToBoneMap.Contains(Packet.RegionTag) ?
        RegionToBoneMap[Packet.RegionTag] : NAME_None; // Use NAME_None if no mapping found

    if (!BoneName.IsNone() && TargetMesh)
    {
        // Apply a visual pulse (e.g., a particle effect) at the bone's location.
        FVector BoneLoc = TargetMesh->GetBoneLocation(BoneName);
        if (HapticPulseParticleSystem)
        {
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HapticPulseParticleSystem, BoneLoc);
        }

        // Optional: Animate skin material pulse (e.g., emissive shimmer) based on haptic intensity.
        // This assumes 'SkinMaterial' is correctly set up as a dynamic instance.
        if (SkinMaterial)
        {
            float PulseIntensity = FMath::Clamp(Packet.Intensity * 5.0f, 0.0f, 1.0f); // Scale intensity for visual effect
            SkinMaterial->SetScalarParameterValue(TEXT("HapticPulseOverlay"), PulseIntensity); // Set material parameter
            // Decay logic for HapticPulseOverlay could be implemented here with a timer or in the shader itself.
        }

        UE_LOG(LogTemp, Log, TEXT("[EmbodiedAvatar] Visual pulse applied at bone %s from region %s"),
            *BoneName.ToString(), *Packet.RegionTag);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[EmbodiedAvatar] Unknown region: %s — no bone mapped for haptic feedback."), *Packet.RegionTag);
    }
}

// ProcessSkinWavefrontBatch: Enqueues a render command to execute the GPU skin processing pass.
void UEmbodiedAvatarComponent::ProcessSkinWavefrontBatch()
{
    if (!bEnableWavefrontSkinProcessing || !SkinStateBuffer.IsValid() || !SkinResponseTexture.IsValid() || DynamicMaterials.Num() == 0) return;
    // Enqueue render command to execute GPU pass on the Render Thread
    // Capture necessary CPU-side data to pass to the render thread lambda
    FWavefrontSkinState StateToUpload = CurrentSkinState; // Copy the current state
    FLinearColor SigilColorToPass = CurrentSigilGlowColor; // Use the stored sigil color

    ENQUEUE_RENDER_COMMAND(FProcessSkinWavefrontCommand)(
        [this, StateToUpload, SigilColorToPass](FRHICommandListImmediate& RHICmdList)
        {
            SCOPED_DRAW_EVENT(RHICmdList, ProcessSkinWavefrontBatch); // For GPU profiler

            FRDGBuilder GraphBuilder(RHICmdList);

            // Create an RDG buffer for the input skin state, uploading the current CPU-side state.
            // This is how you get the FWavefrontSkinState data onto the GPU for the compute shader.
            FRDGBufferRef SkinStateInputBuffer = GraphBuilder.CreateBuffer(SkinStateBuffer->GetDesc(), TEXT("SkinStateInputBuffer"));
            GraphBuilder.QueueBufferUpload(SkinStateInputBuffer, &StateToUpload, sizeof(FWavefrontSkinState));
            // Create an RDG texture for the output skin response.
            FRDGTextureRef SkinResponseOutputTexture = GraphBuilder.CreateTexture(SkinResponseTexture->GetDesc(), TEXT("SkinResponseOutputTexture"));
            // Get a reference to the compute shader
            TShaderMapRef<FWavefrontSkinComputeShader> ComputeShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
            // Set up shader parameters
            FWavefrontSkinParameters* PassParameters = GraphBuilder.AllocParameters<FWavefrontSkinParameters>();
            PassParameters->SkinStateBuffer = GraphBuilder.CreateSRV(SkinStateInputBuffer);
            PassParameters->SkinResponseTexture = GraphBuilder.CreateUAV(SkinResponseOutputTexture);

            // Pass additional parameters to the shader
            PassParameters->EmotionalIntensity = StateToUpload.GetEmotionalPulse();
            PassParameters->SigilGlowStrength = SigilColorToPass.A; // Using Alpha as strength or a separate UPROPERTY
            PassParameters->SkinBaseColor = FVector4f(0.8f, 0.6f, 0.5f, 1.0f); // Example base skin color in shader
            PassParameters->SigilGlowColor = FVector4f(SigilColorToPass.R, SigilColorToPass.G, SigilColorToPass.B, 1.0f);
            // Add the compute shader pass to the render graph
            FComputeShaderUtils::AddPass(
                GraphBuilder,
                RDG_EVENT_NAME("WavefrontSkinComputePass"), // Event name for GPU profiler
                ComputeShader,
                PassParameters,
                FIntVector(1, 1, 1) // Dispatch a single thread group (1x1x1) for our 1x1 texture
            );
            // Enqueue a lambda to read back the result from the GPU *after* the graph executes.
            // This is essential if you need the GPU-computed result on the CPU side.
            // For a single pixel texture, a readback is feasible. For larger textures, you'd
            // typically use it directly in subsequent rendering passes.
            // Note: Readbacks can introduce CPU-GPU stalls, so use sparingly for truly real-time updates.
            AddReadbackCopyPass(GraphBuilder, SkinResponseOutputTexture, [this](FRHICommandListImmediate& RHICmdList, const FTexture2DRHIRef& Backbuffer)
            {
                // This callback runs on the Render Thread after the GPU has finished.
                // You can now read the data from 'Backbuffer' (which is our SkinResponseTexture).
                // Map the texture to a CPU-accessible memory pointer.
                uint32 DestStride = 0;
                float4* TextureData = (float4*)RHICmdList.LockTexture2D(Backbuffer, 0, EResourceLockMode::RLM_ReadOnly, DestStride);

                if (TextureData)
                {
                    // Access the single pixel color
                    FLinearColor GPUSkinResponse = FLinearColor(TextureData[0].x, TextureData[0].y, TextureData[0].z, TextureData[0].w);

                    // Unlock the texture
                    RHICmdList.UnlockTexture2D(Backbuffer, 0, EResourceLockMode::RLM_ReadOnly);

                    // Now, you can use GPUSkinResponse to update your dynamic materials on the Render Thread
                    // or queue another task to update on the Game Thread if absolutely necessary.
                    // For direct material updates, staying on the Render Thread is best.
                    UpdateMaterialParametersBatch_RenderThread(GPUSkinResponse);
                    UE_LOG(LogTemp, Verbose, TEXT("[EmbodiedAvatar] GPU Skin Response: R=%.2f G=%.2f B=%.2f A=%.2f"),
                        GPUSkinResponse.R, GPUSkinResponse.G, GPUSkinResponse.B, GPUSkinResponse.A);
                }
            });

            GraphBuilder.Execute(); // Commit the render graph commands
        });
}

// New function: UpdateMaterialParametersBatch_RenderThread
// This function will be called on the Render Thread to update material parameters.
void UEmbodiedAvatarComponent::UpdateMaterialParametersBatch_RenderThread(FLinearColor GPUSkinResponse)
{
    // This part runs on the Render Thread.
    // Apply the GPU-computed color/emissive to your dynamic materials.
    for (UMaterialInstanceDynamic* Mat : DynamicMaterials)
    {
        if (Mat)
        {
            // Assuming your skin materials have these parameters.
            // You might need to update AliraconsciousnessSkin.usf to use this new data.
            Mat->SetVectorParameterValue(FName("ComputedSkinColor"), GPUSkinResponse.GetClamped());
            Mat->SetScalarParameterValue(FName("ComputedEmissiveStrength"), GPUSkinResponse.A * 5.0f); // Use alpha for emissive
        }
    }
    // If you need to log or perform other Game Thread operations with GPUSkinResponse,
    // you would queue a task back to the Game Thread here.
}


// UpdateMaterialParametersBatch: Applies results from GPU (or CPU) to materials
// This original function was intended to be run on the Game Thread.
// With the Render Graph and Readback, it's better to update materials on the Render Thread directly.
// Renaming this to reflect its original intent or deprecating it in favor of the _RenderThread version.
// For consistency, let's keep the existing name but clarify its purpose now.
void UEmbodiedAvatarComponent::UpdateMaterialParametersBatch()
{
    // This function is still called on the Game Thread.
    // In a full GPU pipeline, this might become a simple check or trigger for CPU-side state.
    // The actual material updates are now handled in UpdateMaterialParametersBatch_RenderThread.
    // If you need CPU-side access to the GPUSkinResponse for other logic, you'd store it
    // from the ReadbackCopyPass lambda and then use it here.
    // For now, this is a placeholder to show where CPU-side logic might happen.
    UE_LOG(LogTemp, Verbose, TEXT("[EmbodiedAvatar] UpdateMaterialParametersBatch (Game Thread) called. Materials updated on Render Thread."));
}


// UpdatePerformanceMetrics: Tracks processing time
void UEmbodiedAvatarComponent::UpdatePerformanceMetrics(float DeltaTime)
{
    ProcessingTimeHistory.Add(DeltaTime); // Add current frame's delta time
    if (ProcessingTimeHistory.Num() > 60) // Keep history for the last 60 frames
    {
        ProcessingTimeHistory.RemoveAt(0); // Remove oldest entry if buffer is full
    }
    float Sum = 0.0f;
    for (float Time : ProcessingTimeHistory)
    {
        Sum += Time; // Sum up all recorded times
    }
    AverageSkinProcessingTime = Sum / ProcessingTimeHistory.Num(); // Calculate average
}
