


#include "KillCOunterComponent.h"

#include "Components/TextBlock.h"
#include "UnrealBrawler/BrawlerCharacter.h"

#pragma region Setup
bool UKillCounterComponent::Initialize()
{
    if(!Super::Initialize()) return false;
    return true;
}

void UKillCounterComponent::NativeConstruct()
{
    Super::NativeConstruct();
}

void UKillCounterComponent::NativeDestruct()
{
    Super::NativeDestruct();
}

void UKillCounterComponent::SetupCounterComponent(const AActor* Actor)
{
    if(IsValid(Actor))
    {
        const ABrawlerCharacter* BrawlerCharacter = Cast<ABrawlerCharacter>(Actor);
        if(BrawlerCharacter->IsEnemy()) return;
        
        if(IsValid(this->CurrentEnemyKilled)) this->CurrentEnemyKilled->SetText(FText::AsNumber(BrawlerCharacter->GetKillCount()));
    }
}
#pragma endregion


#pragma region Events
void UKillCounterComponent::UpdateCounterEvent(const FText& Amount)
{
    if(IsValid(this->CurrentEnemyKilled)) this->CurrentEnemyKilled->SetText(Amount);
}
#pragma endregion