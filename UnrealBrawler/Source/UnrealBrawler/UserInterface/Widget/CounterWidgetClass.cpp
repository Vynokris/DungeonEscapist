


#include "CounterWidgetClass.h"

#include "Components/TextBlock.h"
#include "UnrealBrawler/BrawlerCharacter.h"

bool UCounterWidget::Initialize()
{
    if(!Super::Initialize()) return false;
    return true;
}

void UCounterWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UCounterWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UCounterWidget::SetupCounterComponent(const AActor* Actor)
{
    if(IsValid(Actor))
    {
        const ABrawlerCharacter* BrawlerCharacter = Cast<ABrawlerCharacter>(Actor);
        if(BrawlerCharacter->IsEnemy()) return;
        
        UpdateCounterEvent(FString::FromInt(BrawlerCharacter->GetKillCount()));
    }
}

void UCounterWidget::UpdateCounterEvent(const FString& Amount)
{
    if(IsValid(this->CurrentEnemyKilled)) this->CurrentEnemyKilled->SetText(FText::FromString(Amount));
}

