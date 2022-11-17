


#include "ButtonWidgetClass.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

bool UButtonWidget::Initialize()
{
    if(!Super::Initialize()) return false;

    this->Button->SetCursor(EMouseCursor::Hand);
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

void UButtonWidget::SetText(const FString& Value) const
{
    if(IsValid(this->DisplayedText)) this->DisplayedText->SetText(FText::FromString(Value));
}

UButton* UButtonWidget::GetButton() const
{
    if(IsValid(this->Button)) return this->Button;
    return nullptr;
}
