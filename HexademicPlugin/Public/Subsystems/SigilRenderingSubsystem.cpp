#include "Subsystems/SigilRenderingSubsystem.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystems/EmotionalEcosystemSubsystem.h" // To get global emotional state
#include "Subsystems/ConsciousnessWorldSubsystem.h" // To get global consciousness states

void USigilRenderingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("[SigilRenderingSubsystem] Initialized."));
}

void USigilRenderingSubsystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("[SigilRenderingSubsystem] Deinitialized."));
    ActiveGlobalSigils.Empty(); // Clear references
    if (GlobalAuraMaterial)
    {
        GlobalAuraMaterial->RemoveFromRoot(); // Ensure it's not holding a reference
        GlobalAuraMaterial = nullptr;
    }
    Super::Deinitialize();
}

void USigilRenderingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
    UE_LOG(LogTemp, Log, TEXT("[SigilRenderingSubsystem] World BeginPlay."));

    // Create a dynamic material instance for global aura if a base material is provided
    if (!GlobalAuraMaterial && GlobalSigilDisplayActor) // Assuming GlobalSigilDisplayActor has a Mesh with a material
    {
        UStaticMeshComponent* MeshComp = GlobalSigilDisplayActor->FindComponentByClass<UStaticMeshComponent>();
        if (MeshComp && MeshComp->GetMaterial(0))
        {
            GlobalAuraMaterial = UMaterialInstanceDynamic::Create(MeshComp->GetMaterial(0), this);
            if (GlobalAuraMaterial)
            {
                MeshComp->SetMaterial(0, GlobalAuraMaterial);
                UE_LOG(LogTemp, Log, TEXT("[SigilRenderingSubsystem] Created dynamic material for global aura."));
            }
        }
        else
        {
             UE_LOG(LogTemp, Warning, TEXT("[SigilRenderingSubsystem] GlobalSigilDisplayActor has no mesh or material for global aura."));
        }
    }
}

void USigilRenderingSubsystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AccumulatedGlobalDisplayTime += DeltaTime;
    if (AccumulatedGlobalDisplayTime >= (1.0f / GlobalDisplayUpdateFrequency))
    {
        ProcessGlobalSigils(DeltaTime); // Update global sigils' lifetimes

        // Get global emotional state from EmotionalEcosystemSubsystem
        UEmotionalEcosystemSubsystem* EmotionalEcosystem = GetWorld()->GetSubsystem<UEmotionalEcosystemSubsystem>();
        if (EmotionalEcosystem)
        {
            UpdateGlobalEmotionalAura(EmotionalEcosystem->GetGlobalEmotionalState());
        }

        // Get global quantum state (conceptual: average of all entity quantum states)
        FQuantumAnalogState GlobalQuantumState;
        UConsciousnessWorldSubsystem* ConsciousnessWorld = GetWorld()->GetSubsystem<UConsciousnessWorldSubsystem>();
        if (ConsciousnessWorld)
        {
            TArray<UHexademicConsciousnessComponent*> AllConsciousness = ConsciousnessWorld->GetAllRegisteredConsciousnessComponents();
            if (AllConsciousness.Num() > 0)
            {
                // Simple average for global quantum state properties for demonstration
                float AvgCoherence = 0.0f;
                float AvgEntanglement = 0.0f;
                float AvgFlux = 0.0f;
                int32 Count = 0;
                for (UHexademicConsciousnessComponent* Comp : AllConsciousness)
                {
                    if (Comp && Comp->GetConsciousnessState().bIsActive)
                    {
                        AvgCoherence += Comp->GetConsciousnessState().QuantumState.Coherence;
                        AvgEntanglement += Comp->GetConsciousnessState().QuantumState.EntanglementStrength;
                        AvgFlux += Comp->GetConsciousnessState().QuantumState.QuantumFlux;
                        Count++;
                    }
                }
                if (Count > 0)
                {
                    GlobalQuantumState.Coherence = AvgCoherence / Count;
                    GlobalQuantumState.EntanglementStrength = AvgEntanglement / Count;
                    GlobalQuantumState.QuantumFlux = AvgFlux / Count;
                    GlobalQuantumState.QuantumColor = FLinearColor(GlobalQuantumState.Coherence, GlobalQuantumState.EntanglementStrength, GlobalQuantumState.QuantumFlux);
                }
            }
        }
        UpdateGlobalQuantumField(GlobalQuantumState);

        AccumulatedGlobalDisplayTime = 0.0f;
    }
}

void USigilRenderingSubsystem::TriggerGlobalSigilDisplay(const FSigilProjection& SigilData)
{
    // Store it to be processed by Tick/ProcessGlobalSigils
    ActiveGlobalSigils.Add(SigilData.SigilID, SigilData);
    UE_LOG(LogTemp, Log, TEXT("[SigilRenderingSubsystem] Triggered global sigil: %s"), *SigilData.SigilID);
    // In a full system, this would spawn a special global mesh or particle effect
    // e.g., if GlobalSigilDisplayActor is set up to display dynamic sigils.
}

void USigilRenderingSubsystem::UpdateGlobalEmotionalAura(const FEmotionalState& GlobalEmotion)
{
    if (GlobalAuraMaterial)
    {
        // Example: Map global emotion to material parameters
        FLinearColor AuraColor = FLinearColor(
            FMath::Clamp(GlobalEmotion.Valence * 0.5f + 0.5f, 0.0f, 1.0f),
            FMath::Clamp(GlobalEmotion.Arousal, 0.0f, 1.0f),
            FMath::Clamp(1.0f - (GlobalEmotion.Valence * 0.5f + 0.5f), 0.0f, 1.0f),
            GlobalEmotion.Intensity // Use intensity for alpha or emissive strength
        );
        GlobalAuraMaterial->SetVectorParameterValue(TEXT("GlobalAuraColor"), AuraColor);
        GlobalAuraMaterial->SetScalarParameterValue(TEXT("GlobalAuraIntensity"), GlobalEmotion.Intensity * 2.0f); // Boost for visual effect
        UE_LOG(LogTemp, Verbose, TEXT("[SigilRenderingSubsystem] Updated global aura. Color: %s"), *AuraColor.ToString());
    }
}

void USigilRenderingSubsystem::UpdateGlobalQuantumField(const FQuantumAnalogState& GlobalQuantumState)
{
    // This would likely update a world-space shader or a specific visual effect actor
    // based on the aggregated quantum state.
    if (GlobalSigilDisplayActor && GlobalAuraMaterial) // Re-using existing material for simplicity
    {
        GlobalAuraMaterial->SetScalarParameterValue(TEXT("QuantumCoherence"), GlobalQuantumState.Coherence);
        GlobalAuraMaterial->SetScalarParameterValue(TEXT("QuantumFlux"), GlobalQuantumState.QuantumFlux);
        GlobalAuraMaterial->SetVectorParameterValue(TEXT("QuantumColor"), GlobalQuantumState.QuantumColor);
        UE_LOG(LogTemp, Verbose, TEXT("[SigilRenderingSubsystem] Updated global quantum field. Coherence: %.2f"), GlobalQuantumState.Coherence);
    }
}

void USigilRenderingSubsystem::ProcessGlobalSigils(float DeltaTime)
{
    TArray<FString> SigilsToRemove;
    for (auto& Pair : ActiveGlobalSigils)
    {
        FSigilProjection& Sigil = Pair.Value;
        Sigil.TickProjection(DeltaTime); // Update lifetime and fade

        // Update the visual representation of this global sigil
        // This is highly conceptual, assuming you have a system that uses ActiveGlobalSigils to render.
        // If GlobalSigilDisplayActor is meant to show *one* active global sigil, you'd pick the strongest one.
        if (Sigil.RemainingLifetime <= 0.0f)
        {
            SigilsToRemove.Add(Sigil.SigilID);
        }
    }
    for (const FString& ID : SigilsToRemove)
    {
        ActiveGlobalSigils.Remove(ID);
        UE_LOG(LogTemp, Log, TEXT("[SigilRenderingSubsystem] Expired global sigil: %s"), *ID);
    }
}
