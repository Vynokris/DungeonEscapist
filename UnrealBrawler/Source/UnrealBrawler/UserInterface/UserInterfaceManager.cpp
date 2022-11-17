


#include "UserInterfaceManager.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealBrawler/BrawlerInstance.h"

#pragma region Setup
bool UUserInterfaceManager::Initialize()
{
    if(!Super::Initialize()) return false;
    
    if(IsValid(this->CounterUserWidget))     this->CounterWidget       = Cast<UCounterWidget>(CounterUserWidget);
    if(IsValid(this->HealthBarUserWidget))   this->HealthBarWidget     = Cast<UHealthBarWidget>(HealthBarUserWidget);

    if(IsValid(this->MainMenuUserWidget))    this->MainMenuWidget      = Cast<UMenuWidget>(MainMenuUserWidget);
    if(IsValid(this->WinMenuUserWidget))     this->WinMenuWidget       = Cast<UWinWidget>(WinMenuUserWidget);
    if(IsValid(this->OverMenuUserWidget))    this->OverMenuWidget      = Cast<UOverWidget>(OverMenuUserWidget);
    
    /*if(this->GetBrawlerInstance()->GetGameRestart() == false && !this->GetMainMenu()->IsVisible())*/
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
        //WinMenuWidget->GetRestartButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::RestartGameEvent);
    }
    
    if(IsValid(this->OverMenuWidget))
    {
        OverMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        OverMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMenuGameEvent);
        //OverMenuWidget->GetRestartButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::RestartGameEvent);
    }

    //if(this->BrawlerInstance->GetUserInterfaceManager() == nullptr) this->BrawlerInstance->SetUserInterfaceManager(this);
    
    return true;
}

void UUserInterfaceManager::NativeConstruct()
{
    Super::NativeConstruct();
    this->BrawlerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
}

void UUserInterfaceManager::NativeDestruct()
{
    Super::NativeDestruct();
}
#pragma endregion


#pragma region Events
void UUserInterfaceManager::QuitGameEvent()
{
    this->BrawlerController->ConsoleCommand("quit");
}

void UUserInterfaceManager::PlayGameEvent()
{
    UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if(BrawlerInstance->GetGameRestart()) UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
    BrawlerInstance->SetGameRestart(0);
    
    this->HideOverMenuEvent();
    this->HideWinMenuEvent();
    this->HideMenuGameEvent();
    
    this->GetCounter()->SetVisibility(ESlateVisibility::Visible);
    this->GetHealthBar()->SetVisibility(ESlateVisibility::Visible);
}

/*void UUserInterfaceManager::RestartGameEvent()
{
    this->GetBrawlerInstance()->SetGameRestart(true);
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
    this->PlayGameEvent();
}*/

void UUserInterfaceManager::ShowMenuGameEvent()
{
    if(IsValid(this->MainMenuUserWidget))
    {
        this->MainMenuUserWidget->SetVisibility(ESlateVisibility::Visible);
        this->UpdateNavigation(this->MainMenuUserWidget);

        UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        if(BrawlerInstance->GetGameRestart() == 1)
        {
            UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
            BrawlerInstance->SetGameRestart(0);
        }
    }
}
void UUserInterfaceManager::HideMenuGameEvent()
{
    if(IsValid(this->MainMenuUserWidget)) this->MainMenuUserWidget->SetVisibility(ESlateVisibility::Hidden);
    this->UpdateNavigation(this->MainMenuUserWidget);
}

void UUserInterfaceManager::ShowOverMenuEvent()
{
    if(IsValid(this->OverMenuUserWidget)) this->OverMenuUserWidget->SetVisibility(ESlateVisibility::Visible);
    this->UpdateNavigation(this->OverMenuUserWidget);

    UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    BrawlerInstance->SetGameRestart(1);
}
void UUserInterfaceManager::HideOverMenuEvent()
{
    if(IsValid(this->OverMenuUserWidget)) this->OverMenuUserWidget->SetVisibility(ESlateVisibility::Hidden);
    this->UpdateNavigation(this->OverMenuUserWidget);
}

void UUserInterfaceManager::ShowWinMenuEvent()
{
    if(IsValid(this->WinMenuUserWidget)) this->WinMenuUserWidget->SetVisibility(ESlateVisibility::Visible);
    this->UpdateNavigation(this->WinMenuUserWidget);
}
void UUserInterfaceManager::HideWinMenuEvent()
{
    if(IsValid(this->WinMenuUserWidget)) this->WinMenuUserWidget->SetVisibility(ESlateVisibility::Hidden);
    this->UpdateNavigation(this->WinMenuUserWidget);
}
#pragma endregion


#pragma region Misc 
void UUserInterfaceManager::UpdateNavigation(const UUserWidget* Widget) const
{
    if(IsValid(this->BrawlerController))
    {
        if (Widget->IsVisible())
        {
            this->BrawlerController->SetPause(true);
            this->BrawlerController->SetShowMouseCursor(true);
            return;
        }

        this->BrawlerController->SetPause(false);
        this->BrawlerController->SetShowMouseCursor(false);
    } 
}
#pragma endregion


#pragma region Getter & Setter
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

UMenuWidget* UUserInterfaceManager::GetMainMenu() const
{
    if(IsValid(this->MainMenuWidget)) return this->MainMenuWidget;
    return nullptr;
}

UWinWidget*	UUserInterfaceManager::GetWinMenu()	const
{
    if(IsValid(this->WinMenuWidget)) return this->WinMenuWidget;
    return nullptr;
}

UOverWidget* UUserInterfaceManager::GetOverMenu() const
{
    if(IsValid(this->OverMenuWidget)) return this->OverMenuWidget;
    return nullptr;
}

/*UBrawlerInstance* UUserInterfaceManager::GetBrawlerInstance() const
{
    if(IsValid(this->BrawlerInstance)) return this->BrawlerInstance;
    return nullptr;
}*/
#pragma endregion







