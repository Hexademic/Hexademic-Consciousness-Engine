#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Living/BiologicalNeedsComponent.generated.h"

UCLASS(ClassGroup=(HexademicLiving), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UBiologicalNeedsComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UBiologicalNeedsComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Biological Needs")
    void ConsumeFood(float NutritionValue);

    UFUNCTION(BlueprintCallable, Category = "Biological Needs")
    void ConsumeWater(float HydrationValue);

    UFUNCTION(BlueprintCallable, Category = "Biological Needs")
    void Rest(float RestDuration); // Simulates sleeping/resting

    UFUNCTION(BlueprintPure, Category = "Biological Needs")
    float GetHunger() const { return Hunger; }

    UFUNCTION(BlueprintPure, Category = "Biological Needs")
    float GetThirst() const { return Thirst; }

    UFUNCTION(BlueprintPure, Category = "Biological Needs")
    float GetFatigue() const { return Fatigue; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Biological State")
    float Hunger; // 0.0 (full) to 1.0 (starving)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Biological State")
    float Thirst; // 0.0 (hydrated) to 1.0 (dehydrated)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Biological State")
    float Fatigue; // 0.0 (rested) to 1.0 (exhausted)

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Tuning")
    float HungerRate = 0.01f; // Rate per second hunger increases
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Tuning")
    float ThirstRate = 0.02f; // Rate per second thirst increases
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Tuning")
    float FatigueRate = 0.005f; // Rate per second fatigue increases (when awake)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Biological Tuning")
    float RestRecoveryRate = 0.1f; // Rate per second fatigue decreases (when resting)
};
