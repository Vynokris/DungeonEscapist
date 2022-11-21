#include "BrawlerInstance.h"

#include "Actors/DoorActor.h"
#include "Actors/TropheeActor.h"
#include "Kismet/GameplayStatics.h"

#define INVINCIBILITY_STENCIL_VAL 3

void UBrawlerInstance::Init()
{
    Super::Init();
    IsGameRestart = false;
}

void UBrawlerInstance::Shutdown()
{
    Super::Shutdown();
}

void UBrawlerInstance::StartGameInstance()
{
    Super::StartGameInstance();
}


bool UBrawlerInstance::GetGameRestart() const
{
    return IsGameRestart;
}

void UBrawlerInstance::SetGameRestart(const bool& IsRestart)
{
    IsGameRestart = IsRestart;
}

bool UBrawlerInstance::GetGameWin() const
{
    return IsGameWin;
}

void UBrawlerInstance::SetGameWin(const bool& IsWin)
{
    IsGameWin = IsWin;

    // Open the exit door when the game is won.
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Door", FoundActors);
    ADoorActor* ExitDoor = nullptr;
    for (AActor* Actor : FoundActors)
        if (!ExitDoor || ExitDoor->GetActorLocation().Y < Actor->GetActorLocation().Y)
            ExitDoor = Cast<ADoorActor>(Actor);
    if (IsValid(ExitDoor))
        ExitDoor->MoveDoorEvent();

    // Make the trophee shine.
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), "Trophee", FoundActors);
    if (FoundActors.Num() > 0 && IsValid(FoundActors[0]))
        Cast<ATropheeActor>(FoundActors[0])->GetMesh()->SetCustomDepthStencilValue(INVINCIBILITY_STENCIL_VAL);
}

