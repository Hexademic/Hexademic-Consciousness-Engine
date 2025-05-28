// UMeshPulseShaderComponent.cpp

#include "UMeshPulseShaderComponent.h" #include "NiagaraComponent.h" #include "NiagaraFunctionLibrary.h" #include "Kismet/GameplayStatics.h" #include "Components/SkeletalMeshComponent.h" #include "Engine/World.h"

UUMeshPulseShaderComponent::UUMeshPulseShaderComponent() { PrimaryComponentTick.bCanEverTick = true; PulseFrequency = 1.0f; PulseAmplitude = 1.0f; PulsePhase = 0.0f; SocketName = "HeartSocket"; }

void UUMeshPulseShaderComponent::BeginPlay() { Super::BeginPlay();

SkeletalMesh = Cast<USkeletalMeshComponent>(GetAttachParent());
if (!SkeletalMesh)
{
	UE_LOG(LogTemp, Warning, TEXT("UMeshPulseShaderComponent: No valid skeletal mesh parent found."));
	return;
}

if (NiagaraPulseSystem)
{
	PulseSystem = UNiagaraFunctionLibrary::SpawnSystemAttached(
		NiagaraPulseSystem,
		SkeletalMesh,
		SocketName,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::SnapToTarget,
		true);
}

}

void UUMeshPulseShaderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) { Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

if (!PulseSystem)
	return;

PulsePhase += DeltaTime * PulseFrequency * 2 * PI;
if (PulsePhase > 2 * PI)
	PulsePhase -= 2 * PI;

float PulseStrength = FMath::Sin(PulsePhase) * PulseAmplitude;

PulseSystem->SetFloatParameter(TEXT("PulseStrength"), PulseStrength);

}

void UUMeshPulseShaderComponent::SetPulseProperties(float Frequency, float Amplitude) { PulseFrequency = Frequency; PulseAmplitude = Amplitude; }

