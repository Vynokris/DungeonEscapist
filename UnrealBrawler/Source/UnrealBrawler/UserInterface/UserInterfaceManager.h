

#pragma once

#include "CoreMinimal.h"
#include "Widget/MenuWidgetClass.h"
#include "Widget/OverWidgetClass.h"
#include "Widget/WinWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "Widget/PauseWidget.h"
#include "UserInterfaceManager.generated.h"

class UBrawlerInstance;
UCLASS()
class UNREALBRAWLER_API UUserInterfaceManager : public UUserWidget
{
	GENERATED_BODY()

private:
	UMenuWidget*		MainMenuWidget		= nullptr;
	UPauseWidget*		PauseMenuWidget		= nullptr;
	UOverWidget*		OverMenuWidget		= nullptr;
	UWinWidget*			WinMenuWidget		= nullptr;
	
public:
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		MainMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		WinMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		OverMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		PauseMenuUserWidget		= nullptr;
	
private:
	void UpdateNavigation(const UUserWidget* UserWidget) const;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UMenuWidget*		GetMainMenu()	const;
	UPauseWidget*		GetPauseMenu()	const;
	UOverWidget*		GetOverMenu()	const;
	UWinWidget*			GetWinMenu()	const;
	
	UFUNCTION(BlueprintCallable) void QuitGameEvent();
	UFUNCTION(BlueprintCallable) void PlayGameEvent();
	
	UFUNCTION(BlueprintCallable) void ShowMainMenuEvent();
	UFUNCTION(BlueprintCallable) void HideMainMenuEvent();

	UFUNCTION(BlueprintCallable) void ShowPauseMenuEvent();
	UFUNCTION(BlueprintCallable) void HidePauseMenuEvent();
	
	UFUNCTION(BlueprintCallable) void ShowOverMenuEvent();
	UFUNCTION(BlueprintCallable) void HideOverMenuEvent();
	
	UFUNCTION(BlueprintCallable) void ShowWinMenuEvent();
	UFUNCTION(BlueprintCallable) void HideWinMenuEvent();
};
