#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EmotionTypes.h"
#include "UGestureEmotionInjectorComponent.generated.h"

UCLASS(ClassGroup=(Hexademic), meta=(BlueprintSpawnableComponent))
class HEXADEMIC_API UGestureEmotionInjectorComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGestureEmotionInjectorComponent();

    /** Call this on gesture recognition to inject emotion into lattice */
    UFUNCTION(BlueprintCallable, Category = "Emotion|Injection")
    void InjectGestureEmotion(FName GestureTag, float Intensity = 1.0f);

protected:
    virtual void BeginPlay() override;

private:
    void MapGestureToEmotion(FName GestureTag, float Intensity);

    UPROPERTY(EditAnywhere, Category = "Emotion Mapping")
    TMap<FName, ECoreEmotion> GestureEmotionMap;

    UPROPERTY(EditAnywhere, Category = "Sensitivity")
    float GlobalSensitivity = 1.0f;
};
