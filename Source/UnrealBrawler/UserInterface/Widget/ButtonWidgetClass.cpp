


#include "ButtonWidgetClass.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

#pragma region Setup
bool UButtonWidget::Initialize()
{
    if(!Super::Initialize()) return false;

    Button->SetCursor(EMouseCursor::Hand);
    
    return true;
}

void UButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UButtonWidget::NativeDestruct()
{
    Super::NativeDestruct();
}
#pragma endregion


#pragma region Getter & Setter
void UButtonWidget::SetText(const FText& Value) const
{
    if(IsValid(DisplayedText)) DisplayedText->SetText(Value);
}

UButton* UButtonWidget::GetButton() const
{
    return IsValid(Button) ? Button : nullptr;
}
#pragma endregion