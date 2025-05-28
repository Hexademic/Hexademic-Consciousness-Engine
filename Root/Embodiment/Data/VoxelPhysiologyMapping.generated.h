#pragma once

#include "CoreMinimal.h"
#include "VoxelPhysiologyMapping.generated.h"

USTRUCT(BlueprintType)
struct FPhysiologySnapshot
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HeartRate = 72.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RespiratoryRate = 16.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SweatGlandActivity = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EndorphinLevel = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ArousalLevel = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CortisolLevel = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp = 0.0f;
};

USTRUCT(BlueprintType)
struct FHexAtomicState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AtomType = "H";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EnergyLevel = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmotionalCharge = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector4 QuantumSpin = FVector4(0, 0, 0, 1);
};
