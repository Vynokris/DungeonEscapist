


#include "UserInterfaceManager.h"

#include "CounterWidgetClass.h"
#include "MenuWidgetClass.h"
#include "OverWidgetClass.h"
#include "WinWidgetClass.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealBrawler/Utils/DebugUtils.h"

bool UUserInterfaceManager::Initialize()
{
    if(!Super::Initialize()) return false;

    if(IsValid(this->CounterUserWidget))     this->CounterWidget       = Cast<UCounterWidget>(CounterUserWidget);
    if(IsValid(this->HealthBarUserWidget))   this->HealthBarWidget     = Cast<UHealthBarWidget>(HealthBarUserWidget);

    if(IsValid(this->MainMenuUserWidget))    this->MainMenuWidget      = Cast<UMenuWidget>(MainMenuUserWidget);
    if(IsValid(this->WinMenuUserWidget))     this->WinMenuWidget       = Cast<UWinWidget>(WinMenuUserWidget);
    if(IsValid(this->OverMenuUserWidget))    this->OverMenuWidget      = Cast<UOverWidget>(OverMenuUserWidget);
    
    this->MenuGameEvent(ESlateVisibility::Visible);

    if(IsValid(this->MainMenuWidget))
    {
        MainMenuWidget->GetPlayButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::PlayGameEvent);
        MainMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
    }

    if(IsValid(this->WinMenuWidget))
    {
        WinMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        WinMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::MenuGameEvent);
        WinMenuWidget->GetRestartButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::RestartGameEvent);
    }
    
    if(IsValid(this->OverMenuWidget))
    {
        OverMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        OverMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::MenuGameEvent);
        OverMenuWidget->GetRestartButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::RestartGameEvent);
    }
    
    return true;
}

void UUserInterfaceManager::NativeConstruct()
{
    Super::NativeConstruct();
}

void UUserInterfaceManager::NativeDestruct()
{
    Super::NativeDestruct();
}

UCounterWidget* UUserInterfaceManager::GetCounter() const
{
    if(IsValid(this->CounterWidget)) return this->CounterWidget;
    return nullptr;
}
UHealthBarWidget* UUserInterfaceManager::GetHealthBar() const
{
    if(IsValid(this->HealthBarWidget)) return this->HealthBarWidget;
    return nullptr;
}

void UUserInterfaceManager::QuitGameEvent()
{
    GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void UUserInterfaceManager::PlayGameEvent()
{
    this->MenuGameEvent(ESlateVisibility::Hidden);
    this->OverMenuEvent(ESlateVisibility::Hidden);
    this->WinMenuEvent(ESlateVisibility::Hidden);
}

void UUserInterfaceManager::RestartGameEvent()
{
    this->PlayGameEvent();
}

void UUserInterfaceManager::MenuGameEvent(const ESlateVisibility& Visibility)
{
    if(IsValid(this->MainMenuUserWidget)) this->MainMenuUserWidget->SetVisibility(Visibility);
}

void UUserInterfaceManager::OverMenuEvent(const ESlateVisibility& Visibility)
{
    if(IsValid(this->OverMenuUserWidget)) this->OverMenuUserWidget->SetVisibility(Visibility);
}

void UUserInterfaceManager::WinMenuEvent(const ESlateVisibility& Visibility)
{
    if(IsValid(this->WinMenuUserWidget)) this->WinMenuUserWidget->SetVisibility(Visibility);
}






