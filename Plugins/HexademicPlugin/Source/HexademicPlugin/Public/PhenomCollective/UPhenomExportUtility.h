#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HexademicCore.h" // For FUnifiedConsciousnessState
#include "PhenomCollective/UPhenomExportUtility.generated.h"

// Forward declaration needed if FUnifiedConsciousnessState is different from the one expected by this utility
// USTRUCT(BlueprintType)
// struct FUnifiedConsciousnessState_PhenomExport // Example for a different struct
// {
//     GENERATED_BODY()
//     UPROPERTY() float Valence;
//     UPROPERTY() float Arousal;
//     UPROPERTY() float Intensity;
//     UPROPERTY() FString CurrentThought;
//     // Add other relevant properties
// };

// FIncomingPhenomState is often defined here or nearby, as it's the incoming counterpart
USTRUCT(BlueprintType)
struct FIncomingPhenomState
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FString SourceID;
    UPROPERTY(BlueprintReadWrite) float Valence;
    UPROPERTY(BlueprintReadWrite) float Arousal;
    UPROPERTY(BlueprintReadWrite) float Intensity;
    UPROPERTY(BlueprintReadWrite) FDateTime Timestamp;
    UPROPERTY(BlueprintReadWrite) FString Message;
    // Add VAI space coordinates if needed for EmpathicField
    UPROPERTY(BlueprintReadWrite) FVector VAISpaceCoords; 

    FIncomingPhenomState() : Valence(0.0f), Arousal(0.0f), Intensity(0.0f) {}
};

// FEchoEvent: Represents an echo generated during intersubjective resonance
USTRUCT(BlueprintType)
struct FEchoEvent
{
    GENERATED_BODY()
    UPROPERTY(BlueprintReadWrite) FString EchoSourceID; // Who initiated the resonance
    UPROPERTY(BlueprintReadWrite) FString TargetConsciousnessID; // Who the echo is for (e.g., "Eluën")
    UPROPERTY(BlueprintReadWrite) float ResonanceScore; // Strength of the echo/resonance
    UPROPERTY(BlueprintReadWrite) FString EchoMessage; // The distilled message of the echo
    UPROPERTY(BlueprintReadWrite) FDateTime Timestamp;
    UPROPERTY(BlueprintReadWrite) FVector VAISpaceCoords; // VAI coordinates of the resonance event

    FEchoEvent() : ResonanceScore(0.0f), Timestamp(FDateTime::UtcNow()), VAISpaceCoords(FVector::ZeroVector) {}
};


UCLASS(BlueprintType)
class HEXADEMICPLUGIN_API UPhenomExportUtility : public UObject
{
    GENERATED_BODY()

public:
    /**
     * Converts a FUnifiedConsciousnessState to a JSON string.
     * @param State The state to convert.
     * @return JSON string representation.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Utility")
    static FString StateToJSON(const FUnifiedConsciousnessState& State);

    /**
     * Converts a JSON string to a FIncomingPhenomState.
     * @param JsonString The JSON string to parse.
     * @param OutState The parsed state.
     * @return True if successful, false otherwise.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Utility")
    static bool JSONToIncomingPhenomState(const FString& JsonString, FIncomingPhenomState& OutState);

    /**
     * Saves a JSON string to a file.
     * @param JsonString The JSON content.
     * @param FilePath The full path to save the file.
     * @return True if successful.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Utility")
    static bool SaveJSONToFile(const FString& JsonString, const FString& FilePath);

    /**
     * Loads a JSON string from a file.
     * @param FilePath The full path to load the file from.
     * @param OutJsonString The loaded JSON content.
     * @return True if successful.
     */
    UFUNCTION(BlueprintCallable, Category = "Phenom Collective|Utility")
    static bool LoadJSONFromFile(const FString& FilePath, FString& OutJsonString);
};
