


#include "UserInterfaceManager.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UnrealBrawler/BrawlerInstance.h"
#include "UnrealBrawler/Utils/DebugUtils.h"

#pragma region Setup
bool UUserInterfaceManager::Initialize()
{
    if(!Super::Initialize()) return false;

    if(IsValid(this->MainMenuUserWidget))  this->MainMenuWidget  = Cast<UMenuWidget>(MainMenuUserWidget);
    if(IsValid(this->WinMenuUserWidget))   this->WinMenuWidget   = Cast<UWinWidget>(WinMenuUserWidget);
    if(IsValid(this->OverMenuUserWidget))  this->OverMenuWidget  = Cast<UOverWidget>(OverMenuUserWidget);
    if(IsValid(this->PauseMenuUserWidget)) this->PauseMenuWidget = Cast<UPauseWidget>(PauseMenuUserWidget);

    if(IsValid(this->MainMenuWidget))
    {
        MainMenuWidget->GetPlayButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::PlayGameEvent);
        MainMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
    }

    if(IsValid(this->WinMenuWidget))
    {
        WinMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        WinMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMainMenuEvent);
    }

    if(IsValid(this->PauseMenuWidget))
    {
        PauseMenuWidget->GetResumeButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::HidePauseMenuEvent);
        PauseMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMainMenuEvent);
    }
    
    if(IsValid(this->OverMenuWidget))
    {
        OverMenuWidget->GetQuitButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::QuitGameEvent);
        OverMenuWidget->GetMenuButton()->GetButton()->OnClicked.AddDynamic(this, &UUserInterfaceManager::ShowMainMenuEvent);
    }
    
    return true;
}

void UUserInterfaceManager::NativeConstruct()
{
    Super::NativeConstruct();

    this->ShowMainMenuEvent();
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
    UBrawlerInstance* BrawlerInstance = CastChecked<UBrawlerInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    if(BrawlerInstance->GetGameRestart()) UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
    BrawlerInstance->SetGameRestart(false);

    HideMainMenuEvent();
    HideOverMenuEvent();
    HideWinMenuEvent();

    //BrawlerInstance->GetBrawlerCaracter()->GetHealthBarComponent()->SetVisibility(ESlateVisibility::Visible);
}

void UUserInterfaceManager::ShowMainMenuEvent()
{
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
    if(IsValid(MainMenuWidget))
    {
        MainMenuWidget->PlayFadeOut();
        MainMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(MainMenuWidget);
    }
}

void UUserInterfaceManager::ShowOverMenuEvent()
{
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
    if(IsValid(OverMenuWidget))
    {
        OverMenuWidget->PlayFadeOut();
        OverMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(OverMenuWidget);
    }
}

void UUserInterfaceManager::ShowPauseMenuEvent()
{
    if(IsValid(PauseMenuWidget))
    {
        PauseMenuWidget->SetVisibility(ESlateVisibility::Visible);
        PauseMenuWidget->PlayFadeIn();
        UpdateNavigation(PauseMenuWidget);
    }
}
void UUserInterfaceManager::HidePauseMenuEvent()
{
    if(IsValid(PauseMenuWidget))
    {
        PauseMenuWidget->PlayFadeOut();
        PauseMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(PauseMenuWidget);
    }
}

void UUserInterfaceManager::ShowWinMenuEvent()
{
    if(IsValid(WinMenuWidget))
    {
        WinMenuWidget->SetVisibility(ESlateVisibility::Visible);
        WinMenuWidget->PlayFadeIn();
        UpdateNavigation(WinMenuWidget);
    }
}
void UUserInterfaceManager::HideWinMenuEvent()
{
    if(IsValid(WinMenuWidget))
    {
        WinMenuWidget->PlayFadeOut();
        WinMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        UpdateNavigation(WinMenuWidget);
    }
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
    return IsValid(MainMenuWidget) ? MainMenuWidget : nullptr;
}

UPauseWidget* UUserInterfaceManager::GetPauseMenu() const
{
    return IsValid(PauseMenuWidget) ? PauseMenuWidget : nullptr;
}

UWinWidget*	UUserInterfaceManager::GetWinMenu()	const
{
    return IsValid(WinMenuWidget) ? WinMenuWidget : nullptr;
}

UOverWidget* UUserInterfaceManager::GetOverMenu() const
{
    return IsValid(OverMenuWidget) ? OverMenuWidget : nullptr;
}
#pragma endregion







