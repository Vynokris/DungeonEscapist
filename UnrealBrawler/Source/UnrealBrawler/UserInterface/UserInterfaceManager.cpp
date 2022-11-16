


#include "UserInterfaceManager.h"

#include "CounterWidgetClass.h"
#include "MenuWidgetClass.h"
#include "OverWidgetClass.h"
#include "WinWidgetClass.h"

bool UGameHUD::Initialize()
{
    if(!Super::Initialize()) return false;

    if(IsValid(this->CounterUserWidget))     this->CounterWidget       = Cast<UCounterWidget>(CounterUserWidget);
    if(IsValid(this->HealthBarUserWidget))   this->HealthBarWidget     = Cast<UHealthBarWidget>(HealthBarUserWidget);

    if(IsValid(this->MainMenuUserWidget))    this->MainMenuWidget      = Cast<UMenuWidget>(MainMenuUserWidget);
    if(IsValid(this->WinMenuUserWidget))     this->WinMenuWidget       = Cast<UWinWidget>(WinMenuUserWidget);
    if(IsValid(this->OverMenuUserWidget))    this->OverMenuWidget      = Cast<UOverWidget>(OverMenuUserWidget);
    
    //if(IsValid(this->MainMenuUserWidget)) this->GoToMainMenu();
    
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

void UGameHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    
}

UCounterWidget* UGameHUD::GetCounter() const
{
    if(IsValid(this->CounterWidget)) return this->CounterWidget;
    return nullptr;
}
UHealthBarWidget* UGameHUD::GetHealthBar() const
{
    if(IsValid(this->HealthBarWidget)) return this->HealthBarWidget;
    return nullptr;
}

void UGameHUD::GoToMainMenu()
{
    this->MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
    
    this->WinMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    this->OverMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UGameHUD::GoToOverMenu()
{
    this->OverMenuWidget->SetVisibility(ESlateVisibility::Visible);
    
    this->MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    this->WinMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}

void UGameHUD::GoToWinMenu()
{
    this->WinMenuWidget->SetVisibility(ESlateVisibility::Visible);
    
    this->MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
    this->OverMenuWidget->SetVisibility(ESlateVisibility::Hidden);
}





