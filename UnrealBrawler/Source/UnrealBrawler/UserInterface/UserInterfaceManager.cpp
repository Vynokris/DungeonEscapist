


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
    
    this->ShowMenuGameEvent();

    if(IsValid(this->MainMenuWidget))
    {
        MainMenuWidget->GetPlayButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::PlayGameEvent);
        MainMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
    }

    if(IsValid(this->WinMenuWidget))
    {
        WinMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        WinMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMenuGameEvent);
        WinMenuWidget->GetRestartButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::RestartGameEvent);
    }
    
    if(IsValid(this->OverMenuWidget))
    {
        OverMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        OverMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMenuGameEvent);
        OverMenuWidget->GetRestartButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::RestartGameEvent);
    }

    this->BrawlerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
    
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
    this->ShowMenuGameEvent();
    this->ShowOverMenuEvent();
    this->ShowWinMenuEvent();
}

void UUserInterfaceManager::RestartGameEvent()
{
    this->PlayGameEvent();
}

void UUserInterfaceManager::ShowMenuGameEvent()
{
    if(IsValid(this->MainMenuUserWidget)) this->MainMenuUserWidget->SetVisibility(this->MainMenuUserWidget->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

    if(IsValid(this->BrawlerController))
    {
        if (this->MainMenuUserWidget->IsVisible())
        {
            this->BrawlerController->SetPause(true);
            this->BrawlerController->SetShowMouseCursor(true);
            return;
        }

        this->BrawlerController->SetPause(false);
        this->BrawlerController->SetShowMouseCursor(false);
    } 
}

void UUserInterfaceManager::ShowOverMenuEvent()
{
    if(IsValid(this->OverMenuUserWidget)) this->OverMenuUserWidget->SetVisibility(this->OverMenuUserWidget->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    
    if(this->OverMenuUserWidget->IsVisible())
    {
        if (this->BrawlerController)
        {
            this->BrawlerController->SetPause(true);
            this->BrawlerController->SetShowMouseCursor(true);
        }
    } else
    {
        
    }
}

void UUserInterfaceManager::ShowWinMenuEvent()
{
    if(IsValid(this->WinMenuUserWidget)) this->WinMenuUserWidget->SetVisibility(this->WinMenuUserWidget->IsVisible() ? ESlateVisibility::Hidden : ESlateVisibility::Visible);

    APlayerController* const BrawlerPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
    
    if(this->WinMenuUserWidget->IsVisible())
    {
        if (BrawlerPlayer)
        {
            BrawlerPlayer->SetPause(true);
            BrawlerPlayer->SetShowMouseCursor(true);
        }
    }
}






