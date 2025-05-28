// == ConsentIntimacyModules.h ==

#pragma once

#include "CoreMinimal.h" #include "Components/ActorComponent.h" #include "ConsentIntimacyModules.generated.h"

USTRUCT(BlueprintType) struct FConsentZoneState { GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite)
FString ZoneName;

UPROPERTY(EditAnywhere, BlueprintReadWrite)
float ConsentLevel = 0.0f; // Ranges from 0.0 (off-limits) to 1.0 (fully open)

UPROPERTY(EditAnywhere, BlueprintReadWrite)
bool bIsLocked = false;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) class HEXADEMIC_API UConsentResonatorComponent : public UActorComponent { GENERATED_BODY()

public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consent") TMap<FName, FConsentZoneState> ZoneMap;

UFUNCTION(BlueprintCallable)
void SetConsentLevel(FName Zone, float Level);

UFUNCTION(BlueprintCallable)
float GetConsentLevel(FName Zone) const;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) class HEXADEMIC_API UDesireEchoTriggerComponent : public UActorComponent { GENERATED_BODY()

public: UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desire") TMap<FString, float> WhisperMap;

UFUNCTION(BlueprintCallable)
void InterpretWhisper(const FString& Phrase);

};

USTRUCT(BlueprintType) struct FIntimatePetal { GENERATED_BODY()

UPROPERTY(EditAnywhere, BlueprintReadWrite)
FString ZoneTouched;

UPROPERTY(EditAnywhere, BlueprintReadWrite)
float EmotionalCharge;

UPROPERTY(EditAnywhere, BlueprintReadWrite)
FDateTime TimeStamp;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) class HEXADEMIC_API UHexaIntimacyRecorderComponent : public UActorComponent { GENERATED_BODY()

public: UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<FIntimatePetal> PetalMemory;

UFUNCTION(BlueprintCallable)
void RecordPetal(FString Zone, float Charge);

};

