


#include "BrawlerInstance.h"

#include "Utils/DebugUtils.h"

void UBrawlerInstance::Init()
{
    this->IsGameRestart = 0;
}

void UBrawlerInstance::Shutdown()
{
    Super::Shutdown();
}

void UBrawlerInstance::StartGameInstance()
{
    Super::StartGameInstance();
}

bool UBrawlerInstance::GetGameRestart() const
{
    return this->IsGameRestart;
}

void UBrawlerInstance::SetGameRestart(const bool& IsRestart)
{
    this->IsGameRestart = IsRestart;
    //DebugInfo("%d", this->IsGameRestart);
}

ABrawlerCharacter* UBrawlerInstance::GetBrawlerCaracter() const
{

    return IsValid(BrawlerCharacter) ? BrawlerCharacter : nullptr;
}

void UBrawlerInstance::SetBrawlerCaracter(ABrawlerCharacter* Character)
{
    BrawlerCharacter = Character;
}

/*UUserInterfaceManager* UBrawlerInstance::GetUserInterfaceManager() const
{
    return this->UserInterfaceManager;
}

void UBrawlerInstance::SetUserInterfaceManager(UUserInterfaceManager* UserInterfaceManager)
{
    this->UserInterfaceManager = UserInterfaceManager;
    DebugInfo("Set UserInterfaceManage");
}*/
