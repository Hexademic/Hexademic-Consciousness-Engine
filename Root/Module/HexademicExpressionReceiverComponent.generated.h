#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HexademicExpressionReceiverComponent.generated.h"

USTRUCT(BlueprintType)
struct FHexademicExpressionVector
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<float> Channels; // e.g. [0.8, 0.1, 0.3, 0.9]

    FHexademicExpressionVector()
    {
        Channels.Init(0.f, 8); // Default to 8-channel expression vector
    }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HEXADEMIC_API UHexademicExpressionReceiverComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    UHexademicExpressionReceiverComponent();

    UFUNCTION(BlueprintCallable, Category="Hexademic")
    void ReceiveExpressionVector(const FHexademicExpressionVector& ExpressionData);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hexademic")
    FHexademicExpressionVector CurrentExpression;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    FHexademicExpressionVector SmoothedExpression;
    float InterpolationSpeed = 4.0f;
};
