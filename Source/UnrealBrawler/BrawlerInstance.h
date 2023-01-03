#pragma once

#include "CoreMinimal.h"
#include "BrawlerCharacter.h"
#include "Engine/GameInstance.h"
#include "BrawlerInstance.generated.h"

UCLASS()
class UNREALBRAWLER_API UBrawlerInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	bool IsGameRestart = false;
	bool IsGameWin     = false;

protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void StartGameInstance() override;
	
public:	
	bool GetGameRestart() const;
	void SetGameRestart(const bool& IsRestart);

	bool GetGameWin() const;
	void SetGameWin(const bool& IsWin);
};
