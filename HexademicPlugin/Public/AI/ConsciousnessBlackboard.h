#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h" // Base class for custom Blackboard Key Types
#include "Core/ConsciousnessState.h" // Our custom consciousness state struct
#include "UObject/ObjectMacros.h"

#include "ConsciousnessBlackboard.generated.h"

/**
 * @brief Custom Blackboard Key Type for storing or referencing a FConsciousnessState.
 * This allows Behavior Trees to directly work with and react to the detailed
 * Hexademic consciousness state.
 */
UCLASS(BlueprintType, meta=(DisplayName="ConsciousnessState"))
class HEXADEMICPLUGIN_API UConsciousnessBlackboard : public UBlackboardKeyType
{
    GENERATED_BODY()

public:
    UConsciousnessBlackboard(const FObjectInitializer& ObjectInitializer);

    static const UConsciousnessBlackboard* StaticClassCast(UBlackboardKeyType* KeyType);

    // Casts a raw value from the Blackboard to our custom type
    static FConsciousnessState GetValue(const UConsciousnessBlackboard* KeyType, const uint8* RawData);

    // Sets a value on the Blackboard for our custom type
    static void SetValue(UConsciousnessBlackboard* KeyType, uint8* RawData, const FConsciousnessState& Value);

    virtual EBlackboardKeyType Get}
