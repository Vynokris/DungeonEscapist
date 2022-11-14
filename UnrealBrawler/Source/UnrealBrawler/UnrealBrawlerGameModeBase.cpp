#include "DebugUtils.h"
#include "UnrealBrawlerGameModeBase.h"

#include "GameHUD.h"
#include "EquipmentActor.h"
#include "Blueprint/UserWidget.h"

void AUnrealBrawlerGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    if(IsValid(WidgetGameClass))
    {
        DebugInfo("Found Game HUD!");
        GameHUD = Cast<UGameHUD>(CreateWidget(GetWorld(), WidgetGameClass));
        
        if(GameHUD)
        {
            GameHUD->AddToViewport();
        }
    }
    else
    {
        DebugError("Game HUD not found!");
    }
}

void AUnrealBrawlerGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}