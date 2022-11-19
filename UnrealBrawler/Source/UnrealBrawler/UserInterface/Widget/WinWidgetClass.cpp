


#include "WinWidgetClass.h"

#pragma region Setup
bool UWinWidget::Initialize()
{
    if(!Super::Initialize()) return false;
    
    this->GetMenuButton()->SetText(FText::FromString("Game Menu"));
    this->GetQuitButton()->SetText(FText::FromString("Quit Game"));
    
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
#pragma endregion


#pragma region Misc
void UWinWidget::PlayFadeIn()
{
    if(IsValid(FadeInAnimation)) PlayAnimation(FadeInAnimation, 0, 1);
}

void UWinWidget::PlayFadeOut()
{
    if(IsValid(FadeOutAnimation)) PlayAnimation(FadeOutAnimation, 0, 1);
}
#pragma endregion


#pragma region Getter & Setter
UButtonWidget* UWinWidget::GetQuitButton() const
{
    return IsValid(this->QuitButtonWidget) ? CastChecked<UButtonWidget>(this->QuitButtonWidget) : nullptr;
}

UButtonWidget* UWinWidget::GetMenuButton() const
{
    return IsValid(this->MenuButtonWidget) ? CastChecked<UButtonWidget>(this->MenuButtonWidget) : nullptr;
}

UWidgetAnimation* UWinWidget::GetFadeAnimation(FString AnimationName) const
{
    if(AnimationName != "OUT" || AnimationName != "IN") return nullptr;
    const int AnimationIndex = AnimationName == "IN" ? 0 : 1;
    
    switch (AnimationIndex)
    {
    case 0: return IsValid(FadeInAnimation) ? FadeInAnimation : nullptr;
    case 1: return IsValid(FadeInAnimation) ? FadeInAnimation : nullptr;
    default: return nullptr;
    }
}
#pragma endregion