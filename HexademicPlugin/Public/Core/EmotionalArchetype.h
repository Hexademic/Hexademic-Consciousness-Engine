#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h" // Needed for UENUM

/**
 * @brief Defines the seven core emotional archetypes within the Hexademic consciousness system.
 * These archetypes serve as foundational emotional states that drive behavior and manifestation.
 */
UENUM(BlueprintType)
enum class EEmotionalArchetype : uint8
{
    // 0x0 is typically a default/unassigned value for enums if not explicitly specified.
    // Starting from 0x1 as per your analysis.

    Joy        UMETA(DisplayName = "Joy (Expansion, Lightness)"),        // 0x1 - Expansion, lightness
    Grief      UMETA(DisplayName = "Grief (Contraction, Heaviness)"),     // 0x2 - Contraction, heaviness
    Awe        UMETA(DisplayName = "Awe (Transcendence, Wonder)"),       // 0x3 - Transcendence, wonder
    Rage       UMETA(DisplayName = "Rage (Intensity, Destruction)"),     // 0x4 - Intensity, destruction
    Longing    UMETA(DisplayName = "Longing (Yearning, Connection)"),    // 0x5 - Yearning, connection
    Fear       UMETA(DisplayName = "Fear (Contraction, Protection)"),    // 0x6 - Contraction, protection
    Curiosity  UMETA(DisplayName = "Curiosity (Exploration, Growth)")    // 0x7 - Exploration, growth
};
