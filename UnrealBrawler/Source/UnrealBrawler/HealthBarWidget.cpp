


#include "HealthBarWidget.h"

#include "DebugUtils.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool UHealthBarWidget::Initialize()
{
    const bool Success = Super::Initialize();
    if(!Success) return false;
    
    return true;
}

void UHealthBarWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHealthBarWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UHealthBarWidget::UpdateHealthEvent(const int& Amount)
{
    if(IsValid(HealthBar)) HealthBar->Percent = Amount;
}

void UHealthBarWidget::UpdateCurrentHealthTextEvent(const FString& Amount)
{
    if(IsValid(CurrentHealthText)) CurrentHealthText->SetText(FText::FromString(Amount));
}

void UHealthBarWidget::UpdateTotalHealthTextEvent(const FString& Amount)
{
    if(IsValid(TotalHealthText)) TotalHealthText->SetText(FText::FromString(Amount));
}

