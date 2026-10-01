#include "Components/SigilProjectionComponent.h"
#include "Components/DecalComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/CollisionProfile.h" // For UDecalComponent settings

USigilProjectionComponent::USigilProjectionComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void USigilProjectionComponent::BeginPlay()
{
    Super::BeginPlay();
}

void USigilProjectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    // Iterate through active sigils and tick their lifetime
    TArray<FString> SigilsToRemove;
    for (auto& Pair : ActiveSigilData)
    {
        FSigilProjection& Sigil = Pair.Value;
        Sigil.TickProjection(DeltaTime); // Update lifetime and potentially fade

        if (Sigil.RemainingLifetime <= 0.0f)
        {
            SigilsToRemove.Add(Sigil.SigilID);
        }
        else
        {
            // Update the visual component's material parameters based on faded properties
            if (UPrimitiveComponent* VisualComp = ActiveSigilVisuals.FindRef(Sigil.SigilID))
            {
                if (UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(VisualComp->GetMaterial(0)))
                {
                    UpdateSigilVisualProperties(VisualComp, DynMat, Sigil);
                }
            }
        }
    }

    // Remove expired sigils
    for (const FString& ID : SigilsToRemove)
    {
        RemoveSigil(ID);
    }
}

void USigilProjectionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ClearAllSigils(); // Clean up all visual components
    Super::EndPlay(EndPlayReason);
}

void USigilProjectionComponent::ProjectSigil(const FSigilProjection& SigilData)
{
    if (ActiveSigilData.Contains(SigilData.SigilID))
    {
        // Update existing sigil if it already exists
        UE_LOG(LogTemp, Log, TEXT("[SigilProjection] Updating existing sigil: %s"), *SigilData.SigilID);
        ActiveSigilData[SigilData.SigilID] = SigilData;
        if (UPrimitiveComponent* VisualComp = ActiveSigilVisuals.FindRef(SigilData.SigilID))
        {
            if (UMaterialInstanceDynamic* DynMat = Cast<UMaterialInstanceDynamic>(VisualComp->GetMaterial(0)))
            {
                UpdateSigilVisualProperties(VisualComp, DynMat, SigilData);
            }
        }
        return;
    }

    UPrimitiveComponent* NewVisualComponent = SpawnSigilVisualComponent(SigilData);
    if (NewVisualComponent)
    {
        ActiveSigilData.Add(SigilData.SigilID, SigilData);
        ActiveSigilVisuals.Add(SigilData.SigilID, NewVisualComponent);
        UE_LOG(LogTemp, Log, TEXT("[SigilProjection] Projected new sigil: %s at %s"), *SigilData.SigilID, *SigilData.Location.ToString());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[SigilProjection] Failed to project sigil %s. No valid visual component spawned."), *SigilData.SigilID);
    }
}

void USigilProjectionComponent::RemoveSigil(const FString& SigilID)
{
    if (ActiveSigilData.Contains(SigilID))
    {
        if (UPrimitiveComponent* VisualComp = ActiveSigilVisuals.FindRef(SigilID))
        {
            VisualComp->DestroyComponent();
            UE_LOG(LogTemp, Log, TEXT("[SigilProjection] Destroyed visual component for sigil: %s"), *SigilID);
        }
        ActiveSigilData.Remove(SigilID);
        ActiveSigilVisuals.Remove(SigilID);
        UE_LOG(LogTemp, Log, TEXT("[SigilProjection] Removed sigil: %s"), *SigilID);
    }
}

void USigilProjectionComponent::ClearAllSigils()
{
    TArray<FString> CurrentSigilIDs;
    ActiveSigilData.GetKeys(CurrentSigilIDs);
    for (const FString& ID : CurrentSigilIDs)
    {
        RemoveSigil(ID);
    }
    UE_LOG(LogTemp, Log, TEXT("[SigilProjection] Cleared all sigils."));
}

UPrimitiveComponent* USigilProjectionComponent::SpawnSigilVisualComponent(const FSigilProjection& SigilData)
{
    UPrimitiveComponent* VisualComponent = nullptr;
    AActor* Owner = GetOwner();
    if (!Owner) return nullptr;

    // Prioritize component type based on your setup
    if (DecalComponentClass)
    {
        UDecalComponent* Decal = NewObject<UDecalComponent>(Owner, DecalComponentClass);
        if (Decal)
        {
            Decal->RegisterComponent();
            Decal->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            Decal->SetRelativeLocation(SigilData.Location - Owner->GetActorLocation()); // Relative to owner
            Decal->SetRelativeRotation(SigilData.Rotation);
            Decal->DecalSize = FVector(SigilData.Scale * 50.0f, SigilData.Scale * 50.0f, SigilData.Scale * 50.0f); // Adjust for decal scale
            Decal->SetMaterial(0, DefaultSigilMaterial); // Assign base material
            VisualComponent = Decal;
        }
    }
    else if (ParticleComponentClass)
    {
        UParticleSystemComponent* ParticleComp = NewObject<UParticleSystemComponent>(Owner, ParticleComponentClass);
        if (ParticleComp)
        {
            ParticleComp->RegisterComponent();
            ParticleComp->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            ParticleComp->SetRelativeLocation(SigilData.Location - Owner->GetActorLocation());
            ParticleComp->SetRelativeScale3D(FVector(SigilData.Scale));
            // ParticleComp->SetTemplate(); // Set actual particle system template if needed
            VisualComponent = ParticleComp;
        }
    }
    else if (StaticMeshComponentClass)
    {
        UStaticMeshComponent* StaticMeshComp = NewObject<UStaticMeshComponent>(Owner, StaticMeshComponentClass);
        if (StaticMeshComp)
        {
            StaticMeshComp->RegisterComponent();
            StaticMeshComp->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            StaticMeshComp->SetRelativeLocation(SigilData.Location - Owner->GetActorLocation());
            StaticMeshComp->SetRelativeRotation(SigilData.Rotation);
            StaticMeshComp->SetRelativeScale3D(FVector(SigilData.Scale));
            StaticMeshComp->SetMaterial(0, DefaultSigilMaterial);
            VisualComponent = StaticMeshComp;
        }
    }

    if (VisualComponent && DefaultSigilMaterial)
    {
        UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(DefaultSigilMaterial, this);
        if (DynMat)
        {
            VisualComponent->SetMaterial(0, DynMat);
            UpdateSigilVisualProperties(VisualComponent, DynMat, SigilData);
        }
    }
    
    return VisualComponent;
}

void USigilProjectionComponent::UpdateSigilVisualProperties(UPrimitiveComponent* VisualComponent, UMaterialInstanceDynamic* DynamicMaterial, const FSigilProjection& SigilData)
{
    if (!VisualComponent || !DynamicMaterial) return;

    // Apply color and intensity to material parameters
    DynamicMaterial->SetVectorParameterValue(TEXT("SigilColor"), SigilData.ProjectedColor);
    DynamicMaterial->SetScalarParameterValue(TEXT("SigilIntensity"), SigilData.Intensity);
    
    // Update visual component's scale if it's dynamic
    VisualComponent->SetRelativeScale3D(FVector(SigilData.Scale));

    // You might also update other parameters like pattern, texture, etc., in the material
    // DynamicMaterial->SetTextureParameterValue(TEXT("SigilPattern"), MyTexture);
}
