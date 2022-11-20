


#include "BrawlerInstance.h"

#include "Utils/DebugUtils.h"

void UBrawlerInstance::Init()
{
    Super::Init();
    IsGameRestart = false;
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
    return IsGameRestart;
}

void UBrawlerInstance::SetGameRestart(const bool& IsRestart)
{
    IsGameRestart = IsRestart;
    DebugData("IsGameRestart is now %d ", IsGameWin);
}

bool UBrawlerInstance::GetGameWin() const
{
    return IsGameWin;
}

void UBrawlerInstance::SetGameWin(const bool& IsWin)
{
    IsGameWin = IsWin;
    DebugData("IsGameWin is now %d ", IsGameWin);
}

