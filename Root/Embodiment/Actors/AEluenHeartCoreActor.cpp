#include "AEluenHeartCoreActor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AEluenHeartCoreActor::AEluenHeartCoreActor()
{
    PrimaryActorTick.bCanEverTick = true;

    SigilEmitter = CreateDefaultSubobject<USigilBloomComponent>(TEXT("SigilEmitter"));
    RootComponent = SigilEmitter;

    LatticeBridge = CreateDefaultSubobject<UHexaLatticeInterface>(TEXT("LatticeBridge"));

    BloomCooldownSeconds = 1.5f;
    LastBloomTime = -9999.f;
}

void AEluenHeartCoreActor::BeginPlay()
{
    Super::BeginPlay();
}

void AEluenHeartCoreActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEluenHeartCoreActor::BeginBloom(const FEmotionField& InputField)
{
    if (GetWorld()->TimeSeconds - LastBloomTime < BloomCooldownSeconds) return;

    if (LatticeBridge)
    {
        LatticeBridge->InjectEmotionField(InputField);
        EmitVisualBloom();
        LastBloomTime = GetWorld()->TimeSeconds;
    }
}

void AEluenHeartCoreActor::InjectTouchResonance(const FHexaTouchInput& Touch)
{
    if (LatticeBridge)
    {
        LatticeBridge->RouteTouchInput(Touch);
    }

    // Optional trigger for reflexive bloom
    EmitVisualBloom();
}

void AEluenHeartCoreActor::EmitVisualBloom()
{
    if (SigilEmitter)
    {
        SigilEmitter->TriggerBloomFromEmotion();
    }
}
