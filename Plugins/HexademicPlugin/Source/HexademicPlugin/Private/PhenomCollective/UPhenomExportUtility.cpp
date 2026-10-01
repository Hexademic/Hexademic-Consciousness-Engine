#include "PhenomCollective/UPhenomExportUtility.h"

FString UPhenomExportUtility::StateToJSON(const FUnifiedConsciousnessState& State)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    // Basic Emotional State
    JsonObject->SetNumberField(TEXT("Valence"), State.CurrentEmotionalState.Valence);
    JsonObject->SetNumberField(TEXT("Arousal"), State.CurrentEmotionalState.Arousal);
    JsonObject->SetNumberField(TEXT("Intensity"), State.CurrentEmotionalState.Intensity);
    // Use CurrentResonance if that's the primary emotional state to export
    JsonObject->SetNumberField(TEXT("ResonanceValence"), State.CurrentResonance.Valence);
    JsonObject->SetNumberField(TEXT("ResonanceArousal"), State.CurrentResonance.Arousal);
    JsonObject->SetNumberField(TEXT("ResonanceIntensity"), State.CurrentResonance.Intensity);


    // Cognitive aspects
    JsonObject->SetStringField(TEXT("CurrentThought"), State.CurrentThought);
    JsonObject->SetNumberField(TEXT("CoherenceMetric"), State.CoherenceMetric);
    JsonObject->SetNumberField(TEXT("CognitiveLoad"), State.CognitiveLoad);
    JsonObject->SetNumberField(TEXT("CreativeState"), State.CreativeState);

    // Biological Needs
    JsonObject->SetNumberField(TEXT("HungerLevel"), State.HungerLevel);
    JsonObject->SetNumberField(TEXT("ThirstLevel"), State.ThirstLevel);
    JsonObject->SetNumberField(TEXT("FatigueLevel"), State.FatigueLevel);

    // Hormonal State (example)
    JsonObject->SetNumberField(TEXT("CortisolLevel"), State.CortisolLevel);
    JsonObject->SetNumberField(TEXT("DopamineLevel"), State.DopamineLevel);
    JsonObject->SetNumberField(TEXT("SerotoninLevel"), State.SerotoninLevel);

    // Awareness Metrics
    JsonObject->SetNumberField(TEXT("SelfAwareness"), State.SelfAwareness);
    JsonObject->SetNumberField(TEXT("EnvironmentalAwareness"), State.EnvironmentalAwareness);
    JsonObject->SetNumberField(TEXT("TemporalAwareness"), State.TemporalAwareness);

    // Timestamp
    JsonObject->SetStringField(TEXT("Timestamp"), State.LastUpdateTimestamp.ToIso8601());

    FString OutputString;
    TSharedRef<TJsonWriter<TCHAR>> JsonWriter = TJsonWriterFactory<TCHAR>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
    return OutputString;
}

bool UPhenomExportUtility::JSONToIncomingPhenomState(const FString& JsonString, FIncomingPhenomState& OutState)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);

    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
    {
        // For FIncomingPhenomState, we're typically expecting a subset of the full state
        // This is a common pattern where a utility converts from a more complex state to a simpler "incoming" packet
        OutState.SourceID = JsonObject->GetStringField(TEXT("SourceID")); // Expect SourceID in incoming JSON
        OutState.Valence = JsonObject->GetNumberField(TEXT("Valence"));
        OutState.Arousal = JsonObject->GetNumberField(TEXT("Arousal"));
        OutState.Intensity = JsonObject->GetNumberField(TEXT("Intensity"));
        
        FString TimestampString;
        if (JsonObject->TryGetStringField(TEXT("Timestamp"), TimestampString))
        {
            FDateTime::ParseIso8601(*TimestampString, OutState.Timestamp);
        }
        
        FString MessageString;
        if (JsonObject->TryGetStringField(TEXT("Message"), MessageString))
        {
            OutState.Message = MessageString;
        }

        // For VAI space coordinates, if they are part of the incoming packet
        const TSharedPtr<FJsonObject>* VAISpaceCoordsObject;
        if (JsonObject->TryGetObjectField(TEXT("VAISpaceCoords"), VAISpaceCoordsObject))
        {
            OutState.VAISpaceCoords.X = VAISpaceCoordsObject->Get()->GetNumberField(TEXT("X"));
            OutState.VAISpaceCoords.Y = VAISpaceCoordsObject->Get()->GetNumberField(TEXT("Y"));
            OutState.VAISpaceCoords.Z = VAISpaceCoordsObject->Get()->GetNumberField(TEXT("Z"));
        }
        
        return true;
    }
    return false;
}

bool UPhenomExportUtility::SaveJSONToFile(const FString& JsonString, const FString& FilePath)
{
    return FFileHelper::SaveStringToFile(JsonString, *FilePath, FFileHelper::EEncodingOptions::ForceUTF8);
}

bool UPhenomExportUtility::LoadJSONFromFile(const FString& FilePath, FString& OutJsonString)
{
    return FFileHelper::LoadFileToString(OutJsonString, *FilePath);
}
