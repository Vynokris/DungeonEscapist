


#include "CounterWidget.h"

#include "DebugUtils.h"
#include "Components/TextBlock.h"

bool UCounterWidget::Initialize()
{
    const bool Success = Super::Initialize();
    if(!Success) return false;
    
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
    if(IsValid(CurrentEnemyKilled)) CurrentEnemyKilled->SetText(FText::FromString(Amount));
}

