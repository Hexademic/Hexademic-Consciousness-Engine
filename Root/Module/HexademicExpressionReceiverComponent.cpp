#include "HexademicExpressionReceiverComponent.h"

UHexademicExpressionReceiverComponent::UHexademicExpressionReceiverComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UHexademicExpressionReceiverComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UHexademicExpressionReceiverComponent::ReceiveExpressionVector(const FHexademicExpressionVector& ExpressionData)
{
    CurrentExpression = ExpressionData;
}

void UHexademicExpressionReceiverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (int32 i = 0; i < CurrentExpression.Channels.Num(); i++)
    {
        float Target = CurrentExpression.Channels[i];
        float& Current = SmoothedExpression.Channels[i];

        Current = FMath::FInterpTo(Current, Target, DeltaTime, InterpolationSpeed);

        // 🔁 Hook into facial morphs or gesture anims here
        FString DebugName = FString::Printf(TEXT("Channel[%d]"), i);
        // GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, DebugName + FString::SanitizeFloat(Current));
    }
}
