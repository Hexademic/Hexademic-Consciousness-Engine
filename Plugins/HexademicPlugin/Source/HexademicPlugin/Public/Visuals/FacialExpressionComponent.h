#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Visuals/FacialExpressionComponent.generated.h"

UCLASS(ClassGroup=(HexademicVisuals), meta=(BlueprintSpawnableComponent))
class HEXADEMICPLUGIN_API UFacialExpressionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFacialExpressionComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    UFUNCTION(BlueprintCallable, Category = "Facial Expressions")
    void SetEmotionalExpression(float Valence, float Arousal, float Intensity);

    // Target mesh for applying morph targets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
    TObjectPtr<USkeletalMeshComponent> TargetMesh;

protected:
    // Morph target names for blend shapes (adjust these to your Metahuman/character setup)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Mapping")
    FName HappyMorphTarget = TEXT("M_Facial_Smile"); // Example morph target for happiness
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Mapping")
    FName SadMorphTarget = TEXT("M_Facial_Frown"); // Example morph target for sadness
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Mapping")
    FName ArousalMorphTarget = TEXT("M_Facial_BrowRaise"); // Example morph target for arousal (e.g., surprise/tension)

    // Current target morph weights
    float TargetHappyWeight;
    float TargetSadWeight;
    float TargetArousalWeight;

    // Speed of expression changes
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Facial Tuning")
    float ExpressionBlendSpeed = 5.0f;
};
