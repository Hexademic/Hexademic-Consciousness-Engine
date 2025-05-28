// UMeshPulseShaderComponent.h #pragma once

#include "CoreMinimal.h" #include "Components/ActorComponent.h" #include "NiagaraComponent.h" #include "UMeshPulseShaderComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent)) class HEXADEMICCONSCIOUSNESSENGINE_API UMeshPulseShaderComponent : public UActorComponent { GENERATED_BODY()

public: UMeshPulseShaderComponent();

protected: virtual void BeginPlay() override; void InitializeNiagaraSystem(); void BindSocketLocations();

public: virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pulse")
UNiagaraSystem* PulseNiagaraSystem;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pulse")
FName ChestSocketName = "ChestSocket";

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pulse")
FName HeartSocketName = "HeartSocket";

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pulse")
float PulseStrength = 1.0f;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pulse")
FLinearColor EmotionColor = FLinearColor::White;

UFUNCTION(BlueprintCallable, Category="Pulse")
void UpdatePulseFromEmotionVector(const FVector4& EmotionVec4);

private: UNiagaraComponent* ActivePulseNiagara; USkeletalMeshComponent* OwnerSkeletalMesh; FTransform GetSocketTransformSafe(const FName& SocketName) const; };

