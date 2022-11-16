#include "UnrealBrawlerGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AUnrealBrawlerGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    if(IsValid(WidgetGameClass))
    {
        GameHUD = Cast<UGameHUD>(CreateWidget(GetWorld(), WidgetGameClass));
        if(IsValid(GameHUD)) GameHUD->AddToViewport();

        APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        PlayerControllerRef->SetShowMouseCursor(true);
        PlayerControllerRef->SetPause(true);
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
