


#include "ScoreComponent.h"

#include "Components/TextBlock.h"
#include "UnrealBrawler/BrawlerCharacter.h"

#pragma region Setup
bool UScoreComponent::Initialize()
{
    if(!Super::Initialize()) return false;
    return true;
}

void UScoreComponent::NativeConstruct()
{
    Super::NativeConstruct();
}

void UScoreComponent::NativeDestruct()
{
    Super::NativeDestruct();
}

void UScoreComponent::SetupScoreComponent(const AActor* Actor)
{
    if(IsValid(Actor) && IsValid(CurrentScorePoint))
        CurrentScorePoint->SetText(FText::AsNumber(Cast<ABrawlerCharacter>(Actor)->GetScore()));
}
#pragma endregion


#pragma region Events
void UScoreComponent::UpdateScoreEvent(const FText& Amount)
{
   if(IsValid(CurrentScorePoint)) CurrentScorePoint->SetText(Amount);
}
#pragma endregion

