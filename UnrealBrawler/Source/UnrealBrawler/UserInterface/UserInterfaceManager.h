

#pragma once

#include "CoreMinimal.h"
#include "Widget/MenuWidgetClass.h"
#include "Widget/OverWidgetClass.h"
#include "Widget/WinWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "Widget/HealthBarComponent.h"
#include "Widget/KillCOunterComponent.h"
#include "Widget/PauseWidget.h"
#include "UserInterfaceManager.generated.h"

class UBrawlerInstance;
UCLASS()
class UNREALBRAWLER_API UUserInterfaceManager : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		MainMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		WinMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		OverMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		PauseMenuUserWidget		= nullptr;

	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		KillCounterComponent	= nullptr;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget))	class UUserWidget*		HealthBarComponent		= nullptr;
	
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

	UKillCounterComponent* GetCounterComponent() const;
	UHealthBarComponent* GetHealthBarComponent() const;
	
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

	UFUNCTION(BlueprintCallable) void ShowGameUI();
	UFUNCTION(BlueprintCallable) void HideGameUI();
};
