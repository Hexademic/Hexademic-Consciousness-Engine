#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Body/AutonomicNervousSystemComponent.generated.h"

UCLASS(ClassGroup=(HexademicBody), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UAutonomicNervousSystemComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UAutonomicNervousSystemComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    void SetHeartRate(float NewRate);

    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    float GetHeartRate() const { return CurrentHeartRate; }

    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    void SetBreathingRate(float NewRate);

    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    float GetBreathingRate() const { return CurrentBreathingRate; }

    // Add other autonomic functions like skin conductance, temperature regulation etc.
    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    float GetSkinConductance() const { return CurrentSkinConductance; }
    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    float GetCoreBodyTemperature() const { return CurrentCoreBodyTemperature; }
    UFUNCTION(BlueprintCallable, Category = "Autonomic")
    float GetSkinTemperature() const { return CurrentSkinTemperature; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Autonomic State")
    float CurrentHeartRate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Autonomic State")
    float CurrentBreathingRate;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Autonomic State")
    float CurrentSkinConductance;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Autonomic State")
    float CurrentCoreBodyTemperature;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Autonomic State")
    float CurrentSkinTemperature;

    // Parameters for rate changes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Autonomic Tuning")
    float HeartRateChangeSpeed = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Autonomic Tuning")
    float BreathingRateChangeSpeed = 2.0f;

    // Target values to smoothly interpolate towards
    float TargetHeartRate;
    float TargetBreathingRate;
};
