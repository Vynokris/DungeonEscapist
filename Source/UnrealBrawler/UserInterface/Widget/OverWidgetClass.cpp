


#include "OverWidgetClass.h"

#pragma region Setup
bool UOverWidget::Initialize()
{
    if(!Super::Initialize()) return false;
    
    this->GetQuitButton()->SetText(FText::FromString("Quit Game"));
    this->GetMenuButton()->SetText(FText::FromString("Game Menu"));
    
    return true;
}

void UOverWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UOverWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
#pragma endregion


#pragma region Misc
void UOverWidget::PlayFadeIn()
{
    if(IsValid(FadeInAnimation)) PlayAnimation(FadeInAnimation, 0, 1);
}

void UOverWidget::PlayFadeOut()
{
    if(IsValid(FadeOutAnimation)) PlayAnimation(FadeOutAnimation, 0, 1);
}
#pragma endregion


#pragma region Getter & Setter
UButtonWidget* UOverWidget::GetQuitButton() const
{
    return IsValid(this->QuitButtonWidget) ? CastChecked<UButtonWidget>(this->QuitButtonWidget) : nullptr;
}

UButtonWidget* UOverWidget::GetMenuButton() const
{
    return IsValid(this->MenuButtonWidget) ? CastChecked<UButtonWidget>(this->MenuButtonWidget) : nullptr;
}

UWidgetAnimation* UOverWidget::GetFadeAnimation(FString AnimationName) const
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