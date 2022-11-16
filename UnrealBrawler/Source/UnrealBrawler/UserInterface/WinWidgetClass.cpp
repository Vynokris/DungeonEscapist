


#include "WinWidgetClass.h"

bool UWinWidget::Initialize()
{
    if(!Super::Initialize()) return false;

    this->GetRestartButton()->SetText("Restart Game");
    this->GetMenuButton()->SetText("Game Menu");
    this->GetQuitButton()->SetText("Quit Game");
    
    return true;
}

void UWinWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UWinWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

UButtonWidget* UWinWidget::GetRestartButton() const
{
    if(IsValid(this->RestartButtonWidget)) return CastChecked<UButtonWidget>(this->RestartButtonWidget);
    return nullptr;
}

UButtonWidget* UWinWidget::GetQuitButton() const
{
    if(IsValid(this->QuitButtonWidget)) return CastChecked<UButtonWidget>(this->QuitButtonWidget);
    return nullptr;
}

UButtonWidget* UWinWidget::GetMenuButton() const
{
    if(IsValid(this->MenuButtonWidget)) return CastChecked<UButtonWidget>(this->MenuButtonWidget);
    return nullptr;
}
