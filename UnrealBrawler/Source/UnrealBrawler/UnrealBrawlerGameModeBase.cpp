#include "UnrealBrawlerGameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AUnrealBrawlerGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    
    if(IsValid(WidgetGameClass))
    {
        this->UserInterface = Cast<UUserInterfaceManager>(CreateWidget(GetWorld(), WidgetGameClass));
        if(IsValid(this->UserInterface)) this->UserInterface->AddToViewport();

        APlayerController* PlayerControllerRef = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        PlayerControllerRef->SetShowMouseCursor(true);
        PlayerControllerRef->SetPause(true);
    }
}

void AUnrealBrawlerGameModeBase::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

UUserInterfaceManager* AUnrealBrawlerGameModeBase::GetUserInterface()
{
    return this->UserInterface;
}


