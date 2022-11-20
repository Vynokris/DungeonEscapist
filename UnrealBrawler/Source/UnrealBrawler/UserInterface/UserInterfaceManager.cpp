#include "UserInterfaceManager.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealBrawler/BrawlerInstance.h"

#pragma region Setup
bool UUserInterfaceManager::Initialize()
{
    if(!Super::Initialize()) return false;

    const UMenuWidget* MainMenuWidget = GetMainMenu();
    if(IsValid(MainMenuWidget))
    {
        MainMenuWidget->GetPlayButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::PlayGameEvent);
        MainMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
    }

    const UWinWidget* WinMenuWidget = GetWinMenu();
    if(IsValid(WinMenuWidget))
    {
        WinMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        WinMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMainMenuEvent);
    }

    const UPauseWidget* PauseMenuWidget = GetPauseMenu();
    if(IsValid(PauseMenuWidget))
    {
        PauseMenuWidget->GetResumeButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::HidePauseMenuEvent);
        PauseMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMainMenuEvent);
    }

    const UOverWidget* OverMenuWidget = GetOverMenu();
    if(IsValid(OverMenuWidget))
    {
        OverMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        OverMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMainMenuEvent);
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
#pragma endregion


#pragma region Events
void UUserInterfaceManager::QuitGameEvent()
{
    Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()))->ConsoleCommand("quit");
}

void UUserInterfaceManager::PlayGameEvent()
{
    /*UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if(BrawlerInstance->GetGameRestart())
    {
        UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
        BrawlerInstance->SetGameRestart(false);
    }*/

    HideMainMenuEvent();
}

void UUserInterfaceManager::ShowMainMenuEvent()
{
    HideGameUI();
    UMenuWidget* MainMenuWidget = GetMainMenu();
    if(IsValid(MainMenuWidget))
    {
        MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
        MainMenuWidget->PlayFadeIn();
        UpdateNavigation(MainMenuWidget);

        if(IsValid(GetWorld()))
        {
            UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
            if(BrawlerInstance->GetGameRestart())
            {
                UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
                BrawlerInstance->SetGameRestart(false);
            }
        }
    }
}

void UUserInterfaceManager::HideMainMenuEvent()
{
    ShowGameUI();
    UMenuWidget* MainMenuWidget = GetMainMenu();
    
    if(IsValid(MainMenuWidget))
    {
        MainMenuWidget->PlayFadeOut();
        MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(MainMenuWidget);
    }
}

void UUserInterfaceManager::ShowOverMenuEvent()
{
    HideGameUI();
    UOverWidget* OverMenuWidget = GetOverMenu();
    
    if(IsValid(OverMenuWidget))
    {
        OverMenuWidget->SetVisibility(ESlateVisibility::Visible);
        OverMenuWidget->PlayFadeIn();
        UpdateNavigation(OverMenuWidget);
    }

    UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    BrawlerInstance->SetGameRestart(true);
}

void UUserInterfaceManager::HideOverMenuEvent()
{
    ShowGameUI();
    UOverWidget* OverMenuWidget = GetOverMenu();
    
    if(IsValid(OverMenuWidget))
    {
        OverMenuWidget->PlayFadeOut();
        OverMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(OverMenuWidget);
    }
}

void UUserInterfaceManager::ShowPauseMenuEvent()
{
    HideGameUI();
    UPauseWidget* PauseMenuWidget = GetPauseMenu();
    
    if(IsValid(PauseMenuWidget))
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
        PauseMenuWidget->PlayFadeIn();
        UpdateNavigation(PauseMenuWidget);
    }
}

void UUserInterfaceManager::HidePauseMenuEvent()
{
    ShowGameUI();
    UPauseWidget* PauseMenuWidget = GetPauseMenu();
    
    if(IsValid(PauseMenuWidget))
    {
        PauseMenuWidget->PlayFadeOut();
        PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(PauseMenuWidget);
    }
}

void UUserInterfaceManager::ShowWinMenuEvent()
{
    HideGameUI();
    UWinWidget* WinMenuWidget = GetWinMenu();
    
    if(IsValid(WinMenuWidget))
    {
        WinMenuWidget->SetVisibility(ESlateVisibility::Visible);
        WinMenuWidget->PlayFadeIn();
        UpdateNavigation(WinMenuWidget);
    }
}

void UUserInterfaceManager::HideWinMenuEvent()
{
    ShowGameUI();
    UWinWidget* WinMenuWidget = GetWinMenu();
    
    if(IsValid(WinMenuWidget))
    {
        WinMenuWidget->PlayFadeOut();
        WinMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(WinMenuWidget);
    }
}

void UUserInterfaceManager::ShowGameUI()
{
    if(IsValid(KillCounterComponent)) KillCounterComponent->SetVisibility(ESlateVisibility::Visible);
    if(IsValid(HealthBarComponent)) HealthBarComponent->SetVisibility(ESlateVisibility::Visible);
}

void UUserInterfaceManager::HideGameUI()
{
    if(IsValid(KillCounterComponent)) KillCounterComponent->SetVisibility(ESlateVisibility::Hidden);
    if(IsValid(HealthBarComponent)) HealthBarComponent->SetVisibility(ESlateVisibility::Hidden);
}
#pragma endregion


#pragma region Misc 
void UUserInterfaceManager::UpdateNavigation(const UUserWidget* Widget) const
{
    APlayerController* ABrawlerController = nullptr;
    if(IsValid(GetWorld())) ABrawlerController = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));;
    
    if(IsValid(ABrawlerController))
    {
        if (Widget->IsVisible())
        {
            ABrawlerController->SetPause(true);
            ABrawlerController->SetShowMouseCursor(true);
            return;
        }

        ABrawlerController->SetPause(false);
        ABrawlerController->SetShowMouseCursor(false);
    } 
}
#pragma endregion


#pragma region Getter & Setter
UMenuWidget* UUserInterfaceManager::GetMainMenu() const
{
    return IsValid(MainMenuUserWidget) ? Cast<UMenuWidget>(MainMenuUserWidget) : nullptr;
}

UPauseWidget* UUserInterfaceManager::GetPauseMenu() const
{
    return IsValid(PauseMenuUserWidget) ? Cast<UPauseWidget>(PauseMenuUserWidget) : nullptr;
}

UWinWidget*	UUserInterfaceManager::GetWinMenu()	const
{
    return IsValid(WinMenuUserWidget) ? Cast<UWinWidget>(WinMenuUserWidget) : nullptr;
}

UKillCounterComponent* UUserInterfaceManager::GetCounterComponent() const
{
    return IsValid(KillCounterComponent) ? Cast<UKillCounterComponent>(KillCounterComponent) : nullptr;
}

UHealthBarComponent* UUserInterfaceManager::GetHealthBarComponent() const
{
    return IsValid(HealthBarComponent) ? Cast<UHealthBarComponent>(HealthBarComponent) : nullptr;
}

UOverWidget* UUserInterfaceManager::GetOverMenu() const
{
    return IsValid(OverMenuUserWidget) ? Cast<UOverWidget>(OverMenuUserWidget) : nullptr;
}
#pragma endregion







