


#include "BrawlerInstance.h"

#include "Utils/DebugUtils.h"

void UBrawlerInstance::Init()
{
    this->IsGameRestart = 0;
}

bool UBrawlerInstance::GetGameRestart() const
{
    return this->IsGameRestart;
}

void UBrawlerInstance::SetGameRestart(const bool& IsRestart)
{
    this->IsGameRestart = IsRestart;
    DebugInfo("%d", this->IsGameRestart);
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
