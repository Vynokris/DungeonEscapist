


#include "MenuWidgetClass.h"

#include "Components/Button.h"

#pragma region Setup
bool UMenuWidget::Initialize()
{
    if(!Super::Initialize()) return false;

    this->GetPlayButton()->SetText(FText::FromString("Start Game"));
    this->GetQuitButton()->SetText(FText::FromString("Quit Game"));
    
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
#pragma endregion


#pragma region Misc
void UMenuWidget::PlayFadeIn()
{
    if(IsValid(FadeInAnimation)) PlayAnimation(FadeInAnimation, 0, 1);
}

void UMenuWidget::PlayFadeOut()
{
    if(IsValid(FadeOutAnimation)) PlayAnimation(FadeOutAnimation, 0, 1);
}
#pragma endregion


#pragma region Getter & Setter
UButtonWidget* UMenuWidget::GetPlayButton() const
{
    return IsValid(this->PlayButtonWidget) ? CastChecked<UButtonWidget>(this->PlayButtonWidget) : nullptr;
}

UButtonWidget* UMenuWidget::GetQuitButton() const
{
    return IsValid(this->QuitButtonWidget) ? CastChecked<UButtonWidget>(this->QuitButtonWidget) : nullptr;
}

UWidgetAnimation* UMenuWidget::GetFadeAnimation(FString AnimationName) const
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
