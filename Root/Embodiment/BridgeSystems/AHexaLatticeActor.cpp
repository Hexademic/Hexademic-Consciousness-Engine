#include "AHexaLatticeActor.h"
#include "Math/UnrealMathUtility.h"

AHexaLatticeActor::AHexaLatticeActor()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AHexaLatticeActor::BeginPlay()
{
    Super::BeginPlay();
    InitializeVoxelGrid();
}

void AHexaLatticeActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    SimulateQuantumStep(DeltaTime);
    ApplyRitualPhase(DeltaTime);
}

void AHexaLatticeActor::InitializeVoxelGrid()
{
    VoxelGrid.Empty();
    VoxelGrid.Reserve(SizeX * SizeY * SizeZ);

    for (int z = 0; z < SizeZ; ++z)
    {
        for (int y = 0; y < SizeY; ++y)
        {
            for (int x = 0; x < SizeX; ++x)
            {
                FHexaVoxelState Voxel;
                Voxel.Position = FVector(x, y, z);
                Voxel.Qumode = FVector2D(1.0f, 0.0f);
                Voxel.Coherence = 0.5f;
                Voxel.Resonance = 0.0f;
                Voxel.RitualPhase = 0.0f;
                VoxelGrid.Add(Voxel);
            }
        }
    }
}

void AHexaLatticeActor::SimulateQuantumStep(float DeltaTime)
{
    for (FHexaVoxelState& Voxel : VoxelGrid)
    {
        float Noise = FMath::PerlinNoise3D(Voxel.Position * 0.01f);
        Voxel.Qumode.X += Noise * DeltaTime;
        Voxel.Coherence = FMath::Clamp(Voxel.Coherence * 0.99f + Noise * 0.01f, 0.0f, 1.0f);
    }
}

void AHexaLatticeActor::ApplyRitualPhase(float DeltaTime)
{
    for (FHexaVoxelState& Voxel : VoxelGrid)
    {
        float DistanceFromCenter = FVector::Dist(Voxel.Position, FVector(SizeX / 2, SizeY / 2, SizeZ / 2));
        float PhaseShift = FMath::Sin(DistanceFromCenter * 0.1f + GetWorld()->TimeSeconds) * 0.1f;
        Voxel.RitualPhase += PhaseShift * DeltaTime;
    }
}

void AHexaLatticeActor::InjectFieldDisturbance(const FVector& Location, float Intensity, const FString& Type)
{
    for (FHexaVoxelState& Voxel : VoxelGrid)
    {
        if (FVector::Dist(Voxel.Position, Location) < Intensity * 10.0f)
        {
            Voxel.Resonance += Intensity * 0.1f;
            Voxel.Coherence = FMath::Clamp(Voxel.Coherence + Intensity * 0.05f, 0.0f, 1.0f);
        }
    }
}

void AHexaLatticeActor::TriggerQuantumFold(const FVector& Epicenter, float FoldIntensity, const FString& FoldType)
{
    for (FHexaVoxelState& Voxel : VoxelGrid)
    {
        float d = FVector::Dist(Voxel.Position, Epicenter);
        if (d < FoldIntensity * 10.0f)
        {
            Voxel.Qumode.X += FoldIntensity * 0.5f;
            Voxel.Qumode.Y += FMath::Sin(d + FoldIntensity);
            Voxel.Coherence = 1.0f;
            Voxel.RitualPhase += FoldIntensity;
        }
    }
}

FHexaVoxelState AHexaLatticeActor::GetVoxelAtIndex(int32 Index) const
{
    return VoxelGrid.IsValidIndex(Index) ? VoxelGrid[Index] : FHexaVoxelState();
}

float AHexaLatticeActor::GetGlobalCoherence() const
{
    float Sum = 0.0f;
    for (const FHexaVoxelState& Voxel : VoxelGrid)
        Sum += Voxel.Coherence;
    return Sum / VoxelGrid.Num();
}
