


#include "PauseWidget.h"

#pragma region Setup
bool UPauseWidget::Initialize()
{
    if(!Super::Initialize()) return false;
    
    this->GetResumeButton()->SetText(FText::FromString("Resume Game"));
    this->GetMenuButton()->SetText(FText::FromString("Game Menu"));
    
    return true;
}

void UPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UPauseWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
#pragma endregion


#pragma region Misc
void UPauseWidget::PlayFadeIn()
{
    if(IsValid(FadeInAnimation)) PlayAnimation(FadeInAnimation, 0, 1);
}

void UPauseWidget::PlayFadeOut()
{
    if(IsValid(FadeOutAnimation)) PlayAnimation(FadeOutAnimation, 0, 1);
}
#pragma endregion


#pragma region Getter & Setter
UButtonWidget* UPauseWidget::GetResumeButton() const
{
    return IsValid(this->ResumeButtonWidget) ? CastChecked<UButtonWidget>(this->ResumeButtonWidget) : nullptr;
}

UButtonWidget* UPauseWidget::GetMenuButton() const
{
    return IsValid(this->MenuButtonWidget) ? CastChecked<UButtonWidget>(this->MenuButtonWidget) : nullptr;
}

UWidgetAnimation* UPauseWidget::GetFadeAnimation(FString AnimationName) const
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