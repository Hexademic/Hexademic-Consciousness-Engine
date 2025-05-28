#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UFullBodyAvatarSystem.h"
#include "VoxelPhysiologyMapping.h"
#include "HexaLatticeEmotionBridge.generated.h"

class AHexaLatticeActor;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEXADEMIC24_API UHexaLatticeEmotionBridge : public UActorComponent
{
    GENERATED_BODY()

public:
    UHexaLatticeEmotionBridge();

    UFUNCTION(BlueprintCallable, Category = "Hexademic|Bridge")
    void BindToAvatarSystem(UFullBodyAvatarSystem* AvatarSystemRef);

    UFUNCTION(BlueprintCallable, Category = "Hexademic|Bridge")
    void ConnectToLatticeActor(AHexaLatticeActor* LatticeRef);

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    UFullBodyAvatarSystem* AvatarSystem;

    UPROPERTY()
    AHexaLatticeActor* LatticeActor;

    void OnPhysiologyReceived(const FPhysiologySnapshot& Snapshot);
    void OnEmotionChanged(const FString& Emotion, float Intensity);

    void InjectIntoLattice(const FPhysiologySnapshot& Snapshot);
    void MarkEmotionFold(const FString& Type, float Intensity);
};
