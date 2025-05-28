#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AHexaLatticeActor.generated.h"

USTRUCT(BlueprintType)
struct FHexaVoxelState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector2D Qumode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Coherence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Resonance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RitualPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Position;
};

UCLASS()
class HEXADEMIC24_API AHexaLatticeActor : public AActor
{
    GENERATED_BODY()

public:
    AHexaLatticeActor();

    // Simulation Tick
    virtual void Tick(float DeltaTime) override;

    // Initialization
    virtual void BeginPlay() override;

    // Exposed API to inject physiological disturbance
    UFUNCTION(BlueprintCallable, Category = "Hexademic")
    void InjectFieldDisturbance(const FVector& Location, float Intensity, const FString& Type);

    // Exposed API to trigger quantum fold from emotional resonance
    UFUNCTION(BlueprintCallable, Category = "Hexademic")
    void TriggerQuantumFold(const FVector& Epicenter, float FoldIntensity, const FString& FoldType);

    // Voxel data access for debug tools
    UFUNCTION(BlueprintCallable, Category = "Hexademic")
    FHexaVoxelState GetVoxelAtIndex(int32 Index) const;

    UFUNCTION(BlueprintCallable, Category = "Hexademic")
    float GetGlobalCoherence() const;

protected:
    UPROPERTY(EditAnywhere, Category = "Lattice Dimensions")
    int32 SizeX = 32;

    UPROPERTY(EditAnywhere, Category = "Lattice Dimensions")
    int32 SizeY = 32;

    UPROPERTY(EditAnywhere, Category = "Lattice Dimensions")
    int32 SizeZ = 32;

    UPROPERTY(VisibleAnywhere, Category = "Lattice Buffer")
    TArray<FHexaVoxelState> VoxelGrid;

    void InitializeVoxelGrid();
    void SimulateQuantumStep(float DeltaTime);
    void ApplyRitualPhase(float DeltaTime);
};
