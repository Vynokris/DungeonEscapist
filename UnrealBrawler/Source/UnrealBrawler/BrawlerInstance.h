

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BrawlerInstance.generated.h"

UCLASS()
class UNREALBRAWLER_API UBrawlerInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	bool IsGameRestart = 0;
	//UUserInterfaceManager* UserInterfaceManager = nullptr;

protected:
	virtual void Init() override;
	
public:
	bool GetGameRestart() const;
	void SetGameRestart(const bool& IsRestart);

	/*UUserInterfaceManager* GetUserInterfaceManager() const;
	void SetUserInterfaceManager(UUserInterfaceManager* UserInterfaceManager);*/
};
