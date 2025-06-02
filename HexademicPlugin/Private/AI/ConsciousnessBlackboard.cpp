#include "AI/ConsciousnessBlackboard.h"
#include "BehaviorTree/BlackboardComponent.h" // For UBlackboardComponent

UConsciousnessBlackboard::UConsciousnessBlackboard(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // Constructor
}

// Static function to cast to this custom key type
const UConsciousnessBlackboard* UConsciousnessBlackboard::StaticClassCast(UBlackboardKeyType* KeyType)
{
    return Cast<UConsciousnessBlackboard>(KeyType);
}

// Function to get the FConsciousnessState value from the Blackboard
FConsciousnessState UConsciousnessBlackboard::GetValue(const UConsciousnessBlackboard* KeyType, const uint8* RawData)
{
    if (RawData)
    {
        // Assuming RawData directly points to the FConsciousnessState struct
        return *(reinterpret_cast<const FConsciousnessState*>(RawData));
    }
    return FConsciousnessState(); // Return default if invalid
}

// Function to set the FConsciousnessState value on the Blackboard
void UConsciousnessBlackboard::SetValue(UConsciousnessBlackboard* KeyType, uint8* RawData, const FConsciousnessState& Value)
{
    if (RawData)
    {
        // Copy the value directly into the raw data buffer
        *(reinterpret_cast<FConsciousnessState*>(RawData)) = Value;
    }
}

// These functions define how the custom key type behaves in the editor and at runtime
FString UConsciousnessBlackboard::DescribeValue(const UBlackboardComponent& OwnerComp, const uint8* RawData) const
{
    const FConsciousnessState State = GetValue(this, RawData);
    return State.GetDescription(); // Use the FConsciousnessState's own description
}

bool UConsciousnessBlackboard::TestValue(const UBlackboardComponent& OwnerComp, const uint8* RawData, EBasicKeyOperation::Type Op) const
{
    // This is where you define how a condition check on this Blackboard key would work.
    // E.g., check if Vitality > 0.5, or DominantEmotion == Joy.
    // For now, a very basic implementation.
    const FConsciousnessState State = GetValue(this, RawData);
    switch (Op)
    {
        case EBasicKeyOperation::Set: return true; // Always true if it's set
        case EBasicKeyOperation::NotSet: return false; // Always false if it's not set (handled by BT)
        case EBasicKeyOperation::Equal:
            // Example: Check if Vitality is equal to a target value (not very useful for floats)
            // Or if DominantEmotion matches a target enum.
            // This would require a "comparison target" value to be stored in the custom key data.
            return true; // Placeholder
        default: break;
    }
    return false;
}

bool UConsciousnessBlackboard::CanStoreValue(EBlackboardKey::Type InKeyType) const
{
    // We can only store a UObject reference (which FConsciousnessState is not directly)
    // or a "struct" type if blackboard supported it directly.
    // Custom key types are often used for primitive types or simple structs that fit predefined sizes.
    // For a complex struct like FConsciousnessState, it's usually stored by value.
    return InKeyType == EBlackboardKey::Struct; // Or EBlackboardKey::FVector if it's a small struct, etc.
}

TSharedPtr<FBlackboardKeyType> UConsciousnessBlackboard::Clone(UBlackboardKeyType* KeyType) const
{
    return MakeShareable(new FBlackboardKeyType(KeyType->KeyType)); // Simple clone
}

// No .generated.h for custom blackboard keys needed usually unless they also have UFUNCTIONs.
// But we include it to prevent warnings in Unreal's header tool sometimes.
