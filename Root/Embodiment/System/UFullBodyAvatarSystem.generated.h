#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VoxelPhysiologyMapping.h"
#include "UFullBodyAvatarSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPhysiologyUpdate, const FPhysiologySnapshot&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnEmotionalStateChange, const FString& /*emotion*/, float /*intensity*/);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEXADEMIC24_API UFullBodyAvatarSystem : public UActorComponent
{
    GENERATED_BODY()

public:
    UFullBodyAvatarSystem();

    UPROPERTY(BlueprintAssignable)
    FOnPhysiologyUpdate OnPhysiologyUpdate;

    UPROPERTY(BlueprintAssignable)
    FOnEmotionalStateChange OnEmotionalStateChange;

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    float GetHeartRate() const { return PhysiologyParams.HeartRate; }

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    float GetBreathRate() const { return PhysiologyParams.RespiratoryRate; }

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    float GetSkinConductance() const { return PhysiologyParams.SweatGlandActivity; }

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    FVector GetEmotionalVector() const;

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    float GetCoherenceIndex() const;

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    void BroadcastPhysiologyUpdate();

    UFUNCTION(BlueprintCallable, Category = "Hexademic Integration")
    void BroadcastEmotionalStateChange(const FString& Emotion, float Intensity);

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    UPROPERTY(EditAnywhere, Category = "Physiology")
    FPhysiologySnapshot PhysiologyParams;

    void UpdatePhysiologyInternal();
    void UpdateEmotionalStateInternal(const FString& EmotionType, float NewIntensity);
};
