#include "HexademicWavefrontAPI.h"
#include "RHICommandList.h"
#include "RenderGraphUtils.h"
#include "GlobalShader.h"
#include "Shader.h"
#include "Engine/Engine.h" // For GEngine->CreatePooledRenderBuffer/Target
#include "RHIDefinitions.h" // For ERHIAccess

// IMPLEMENT_GLOBAL_SHADER for dummy shaders (needed for compilation)
// In a real project, these would point to actual .usf files.
// For now, these are just allowing the C++ to compile.
IMPLEMENT_GLOBAL_SHADER(UHexademicWavefrontAPI::FWavefrontSigilComputeShader, "/Engine/Private/WavefrontComputeShaders.usf", "MainSigilProcessing", SF_Compute);
IMPLEMENT_GLOBAL_SHADER(UHexademicWavefrontAPI::FHexademicGemSynthesisShader, "/Engine/Private/WavefrontComputeShaders.usf", "SynthesizeGems", SF_Compute);

UHexademicWavefrontAPI::UHexademicWavefrontAPI()
{
    PrimaryComponentTick.bCanEverTick = true;
    bWantsInitializeComponent = true;
}

void UHexademicWavefrontAPI::BeginPlay()
{
    Super::BeginPlay();
    InitializeWavefrontProcessing(); // Initialize GPU resources on game start
}
void UHexademicWavefrontAPI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ShutdownWavefrontProcessing(); // Clean up GPU resources on game end
    Super::EndPlay(EndPlayReason);
}

void UHexademicWavefrontAPI::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    // Periodically process wavefront on GPU
    AccumulatedWavefrontTime += DeltaTime;
    if (AccumulatedWavefrontTime >= (1.0f / WavefrontUpdateRate))
    {
        ProcessWavefrontGPU();
        AccumulatedWavefrontTime = 0.0f;
    }

    // Periodically attempt to synthesize gems
    AccumulatedGemSynthesisTime += DeltaTime;
    if (AccumulatedGemSynthesisTime >= (1.0f / GemSynthesisFrequency))
    {
        SynthesizeHexademicGems();
        AccumulatedGemSynthesisTime = 0.0f;
    }
}

void UHexademicWavefrontAPI::AddSigilNode(const FPackedHexaSigilNode& NewSigil)
{
    if (ActiveSigilNodes.Num() < MaxSigilNodes)
    {
        ActiveSigilNodes.Add(NewSigil);
        UE_LOG(LogTemp, Log, TEXT("[WavefrontAPI] Added Sigil Node: %s"), *NewSigil.SigilID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[WavefrontAPI] Max Sigil Nodes reached, cannot add new sigil: %s"), *NewSigil.SigilID);
    }
}

void UHexademicWavefrontAPI::RemoveSigilNode(const FString& SigilID)
{
    ActiveSigilNodes.RemoveAll([&](const FPackedHexaSigilNode& Node) {
        return Node.SigilID == SigilID;
    });
    UE_LOG(LogTemp, Log, TEXT("[WavefrontAPI] Removed Sigil Node: %s"), *SigilID);
}

void UHexademicWavefrontAPI::ProcessWavefrontGPU()
{
    if (ActiveSigilNodes.Num() == 0 || !SigilNodesBuffer.IsValid() || !WavefrontOutputTexture.IsValid()) return;
    // Enqueue render command to execute GPU pass on the Render Thread
    ENQUEUE_RENDER_COMMAND(FProcessWavefrontGPUCommand)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            FRDGBuilder GraphBuilder(RHICmdList);
            ExecuteWavefrontComputePass(GraphBuilder); // Execute the sigil processing pass
            GraphBuilder.Execute(); // Commit the render graph commands
            // After execution, if needed, read back data or update CPU copies
        });
}

void UHexademicWavefrontAPI::SynthesizeHexademicGems()
{
    if (ActiveSigilNodes.Num() == 0 || !SigilNodesBuffer.IsValid()) return;
    // Enqueue render command for gem synthesis
    ENQUEUE_RENDER_COMMAND(FSynthesizeHexademicGemsCommand)(
        [this](FRHICommandListImmediate& RHICmdList)
        {
            FRDGBuilder GraphBuilder(RHICmdList);
            ExecuteGemSynthesisComputePass(GraphBuilder); // Execute gem synthesis pass
            GraphBuilder.Execute(); // Commit commands
            // Read back newly synthesized gems (conceptual)
        });
}

void UHexademicWavefrontAPI::InitializeWavefrontProcessing()
{
    // Create GPU buffer for Sigil Nodes
    SigilNodesBuffer = GEngine->CreatePooledRenderBuffer(
        FRDGBufferDesc::CreateStructuredDesc(sizeof(FPackedHexaSigilNode), MaxSigilNodes),
        TEXT("WavefrontSigilNodesBuffer"),
        ERHIAccess::SRVCompute | ERHIAccess::UAVCompute // Both read and write
    );
    // Create output texture for general wavefront effects/visualization
    WavefrontOutputTexture = GEngine->CreatePooledRenderTarget(
        FRenderTargetDesc(1024, 1024, PF_FloatRGBA).SetClear(FClearValue::Transparent), // Example resolution
        TEXT("WavefrontOutputTexture")
    );
    UE_LOG(LogTemp, Log, TEXT("[WavefrontAPI] Initialized GPU resources."));
}

void UHexademicWavefrontAPI::ShutdownWavefrontProcessing()
{
    SigilNodesBuffer.SafeRelease();
    WavefrontOutputTexture.SafeRelease();
    UE_LOG(LogTemp, Log, TEXT("[WavefrontAPI] Shut down GPU resources."));
}

// ExecuteWavefrontComputePass: Dispatches the sigil processing compute shader.
void UHexademicWavefrontAPI::ExecuteWavefrontComputePass(FRDGBuilder& GraphBuilder)
{
    if (ActiveSigilNodes.Num() == 0) return;
    // Upload current CPU-side sigil data to a temporary RDG buffer
    FRDGBufferRef SigilNodesInputBuffer = GraphBuilder.CreateBuffer(SigilNodesBuffer->GetDesc(), TEXT("SigilNodesInputBuffer"));
    GraphBuilder.QueueBufferUpload(SigilNodesInputBuffer, ActiveSigilNodes.GetData(), ActiveSigilNodes.Num() * sizeof(FPackedHexaSigilNode));

    // Create an output buffer (if sigils are modified and written back)
    FRDGBufferRef SigilNodesOutputBuffer = GraphBuilder.CreateBuffer(SigilNodesBuffer->GetDesc(), TEXT("SigilNodesOutputBuffer"));
    FWavefrontSigilParameters* PassParameters = GraphBuilder.AllocParameters<FWavefrontSigilParameters>();
    PassParameters->SigilNodesInput = GraphBuilder.CreateSRV(SigilNodesInputBuffer);
    PassParameters->SigilNodesOutput = GraphBuilder.CreateUAV(SigilNodesOutputBuffer);
    PassParameters->WavefrontOutput = GraphBuilder.CreateUAV(GraphBuilder.CreateTexture(WavefrontOutputTexture->GetDesc(), TEXT("WavefrontOutputRDGTexture")));
    PassParameters->NumSigilNodes = ActiveSigilNodes.Num();
    PassParameters->DeltaTime = GetWorld()->GetDeltaSeconds();
    FComputeShaderUtils::AddComputeShaderPass(
        GraphBuilder,
        GET_GLOBAL_SHADER_MAP(GMaxRHIFeatureLevel)->GetShader<FWavefrontSigilComputeShader>(),
        PassParameters,
        FIntVector(FMath::DivideAndRoundUp(ActiveSigilNodes.Num(), 64), 1, 1) // Dispatch groups based on sigil count
    );
    // After compute, if sigils were modified on GPU, read back or update the pooled buffer
    GraphBuilder.CopyToPooledBuffer(SigilNodesOutputBuffer, SigilNodesBuffer);
    // If you need to read back to CPU, this would be a separate command/readback queue.
}

// ExecuteGemSynthesisComputePass: Dispatches the gem synthesis compute shader.
void UHexademicWavefrontAPI::ExecuteGemSynthesisComputePass(FRDGBuilder& GraphBuilder)
{
    if (ActiveSigilNodes.Num() == 0) return;
    // Upload current CPU-side sigil data (read-only for synthesis)
    FRDGBufferRef SigilNodesInputBuffer = GraphBuilder.CreateBuffer(SigilNodesBuffer->GetDesc(), TEXT("SigilNodesInputBuffer"));
    GraphBuilder.QueueBufferUpload(SigilNodesInputBuffer, ActiveSigilNodes.GetData(), ActiveSigilNodes.Num() * sizeof(FPackedHexaSigilNode));

    // Create an output buffer for new gems
    FRDGBufferRef SynthesizedGemsOutputBuffer = GraphBuilder.CreateBuffer(
        FRDGBufferDesc::CreateStructuredDesc(sizeof(FHexademicGem), MaxSigilNodes), // Max possible gems = max sigils
        TEXT("SynthesizedGemsOutputBuffer")
    );
    FHexademicGemSynthesisParameters* PassParameters = GraphBuilder.AllocParameters<FHexademicGemSynthesisParameters>();
    PassParameters->SigilNodesInput = GraphBuilder.CreateSRV(SigilNodesInputBuffer);
    PassParameters->SynthesizedGemsOutput = GraphBuilder.CreateUAV(SynthesizedGemsOutputBuffer);
    PassParameters->NumSigilNodes = ActiveSigilNodes.Num();
    PassParameters->MaxGems = MaxSigilNodes; // Placeholder for max gems to create in one pass

    FComputeShaderUtils::AddComputeShaderPass(
        GraphBuilder,
        GET_GLOBAL_SHADER_MAP(GMaxRHIFeatureLevel)->GetShader<FHexademicGemSynthesisShader>(),
        PassParameters,
        FIntVector(FMath::DivideAndRoundUp(ActiveSigilNodes.Num(), 64), 1, 1) // Dispatch based on sigil count
    );
    // Read back results: This is a complex part. In a real scenario, you'd
    // need a readback queue or a staging buffer to get the data from GPU to CPU.
    // For now, we'll conceptually assume new gems are generated and added.
    // TArray<FHexademicGem> NewGemsFromGPU;
    // ... Read back from SynthesizedGemsOutputBuffer ...
    // SynthesizedGems.Append(NewGemsFromGPU);
    // Remove duplicates or low-coherence gems.
    UE_LOG(LogTemp, Log, TEXT("[WavefrontAPI] Gem synthesis pass dispatched."));
    // Add a dummy gem for testing purposes until actual GPU readback is implemented
    if (SynthesizedGems.Num() < 5) // Limit dummy gems
    {
        FHexademicGem DummyGem;
        DummyGem.GemID = FString::Printf(TEXT("DummyGem_%f"), FApp::GetCurrentTime());
        DummyGem.CoherenceRating = FMath::FRand();
        DummyGem.EnergeticSignature = FMath::FRand();
        DummyGem.GemColor = FLinearColor::MakeRandomColor();
        DummyGem.CreationTimestamp = FDateTime::UtcNow();
        SynthesizedGems.Add(DummyGem);
    }
}
