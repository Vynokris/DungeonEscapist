


#include "GameHUD.h"

#include "CounterWidget.h"

bool UGameHUD::Initialize()
{
    const bool Success = Super::Initialize();
    if(!Success) return false;
    
    return true;
}

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();
}

void UGameHUD::NativeDestruct()
{
    Super::NativeDestruct();
}

UCounterWidget* UGameHUD::GetCounter() const
{
    return CastChecked<UCounterWidget>(CounterWidget);
}
UHealthBarWidget* UGameHUD::GetHealthBar() const
{
    return CastChecked<UHealthBarWidget>(HealthBarWidget);
}


