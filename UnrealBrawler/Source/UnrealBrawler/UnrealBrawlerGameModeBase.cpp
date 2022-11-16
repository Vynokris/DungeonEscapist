#include "UnrealBrawlerGameModeBase.h"

#include "DebugUtils.h"
#include "GameHUD.h"
#include "EquipmentActor.h"
#include "Blueprint/UserWidget.h"

void AUnrealBrawlerGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    if(IsValid(WidgetGameClass))
    {
        GameHUD = Cast<UGameHUD>(CreateWidget(GetWorld(), WidgetGameClass));
        if(IsValid(GameHUD)) GameHUD->AddToViewport();
    }
}

void AUnrealBrawlerGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

UGameHUD* AUnrealBrawlerGameModeBase::GetGameHUD()
{
    return GameHUD;
}
