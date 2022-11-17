


#include "MenuWidgetClass.h"

#include "Components/Button.h"

bool UMenuWidget::Initialize()
{
    if(!Super::Initialize()) return false;

    this->GetPlayButton()->SetText("Start Game");
    this->GetQuitButton()->SetText("Quit Game");
    
    return true;
}

void UMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMenuWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

UButtonWidget* UMenuWidget::GetPlayButton() const
{
    if(IsValid(this->PlayButtonWidget)) return CastChecked<UButtonWidget>(this->PlayButtonWidget);
    return nullptr;
}

UButtonWidget* UMenuWidget::GetQuitButton() const
{
    if(IsValid(this->QuitButtonWidget)) return CastChecked<UButtonWidget>(this->QuitButtonWidget);
    return nullptr;
}