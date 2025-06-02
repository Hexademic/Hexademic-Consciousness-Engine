#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Body/HormoneAffectBridgeComponent.generated.h"

UCLASS(ClassGroup=(HexademicBody), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UHormoneAffectBridgeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UHormoneAffectBridgeComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    void AdjustCortisol(float DeltaAmount);
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    float GetCurrentCortisol() const { return CortisolLevel; }

    UFUNCTION(BlueprintCallable, Category = "Hormones")
    void AdjustDopamine(float DeltaAmount);
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    float GetCurrentDopamine() const { return DopamineLevel; }

    UFUNCTION(BlueprintCallable, Category = "Hormones")
    void AdjustSerotonin(float DeltaAmount);
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    float GetCurrentSerotonin() const { return SerotoninLevel; }

    UFUNCTION(BlueprintCallable, Category = "Hormones")
    void AdjustAdrenaline(float DeltaAmount);
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    float GetCurrentAdrenaline() const { return AdrenalineLevel; }

    UFUNCTION(BlueprintCallable, Category = "Hormones")
    void AdjustOxytocin(float DeltaAmount);
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    float GetCurrentOxytocin() const { return OxytocinLevel; }

    UFUNCTION(BlueprintCallable, Category = "Hormones")
    void AdjustMelatonin(float DeltaAmount);
    UFUNCTION(BlueprintCallable, Category = "Hormones")
    float GetCurrentMelatonin() const { return MelatoninLevel; }


protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hormonal State")
    float CortisolLevel; // Stress hormone
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hormonal State")
    float DopamineLevel; // Reward/pleasure neurotransmitter
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hormonal State")
    float SerotoninLevel; // Mood/well-being neurotransmitter
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hormonal State")
    float AdrenalineLevel; // Fight or flight
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hormonal State")
    float OxytocinLevel; // Bonding hormone
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hormonal State")
    float MelatoninLevel; // Sleep regulation

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hormone Decay")
    float HormoneDecayRate = 0.05f; // Universal decay rate for all hormones
};
