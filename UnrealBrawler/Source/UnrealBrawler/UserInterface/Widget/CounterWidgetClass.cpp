


#include "CounterWidgetClass.h"

#include "Components/TextBlock.h"

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

void UCounterWidget::UpdateCounterEvent(const FString& Amount)
{
    if(IsValid(this->CurrentEnemyKilled)) this->CurrentEnemyKilled->SetText(FText::FromString(Amount));
}

