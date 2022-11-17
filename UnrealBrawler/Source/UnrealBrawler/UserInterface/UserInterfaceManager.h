

#pragma once

#include "CoreMinimal.h"
#include "Widget/CounterWidgetClass.h"
#include "Widget/HealthBarWidgetClass.h"
#include "Widget/MenuWidgetClass.h"
#include "Widget/OverWidgetClass.h"
#include "Widget/WinWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "UserInterfaceManager.generated.h"

class UBrawlerInstance;
UCLASS()
class UNREALBRAWLER_API UUserInterfaceManager : public UUserWidget
{
	GENERATED_BODY()

private:
	UCounterWidget*		CounterWidget		= nullptr;
	UHealthBarWidget*	HealthBarWidget		= nullptr;

	UMenuWidget*		MainMenuWidget		= nullptr;
	UWinWidget*			WinMenuWidget		= nullptr;
	UOverWidget*		OverMenuWidget		= nullptr;

	APlayerController*	BrawlerController	= nullptr;
	
public:
	UPROPERTY(EditAnywhere, Category = "HUD UI", meta = (BindWidget))	class UUserWidget*		CounterUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "HUD UI", meta = (BindWidget))	class UUserWidget*		HealthBarUserWidget		= nullptr;

	UPROPERTY(EditAnywhere, Category = "MENU UI", meta = (BindWidget))	class UUserWidget*		MainMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "MENU UI", meta = (BindWidget))	class UUserWidget*		WinMenuUserWidget		= nullptr;
	UPROPERTY(EditAnywhere, Category = "MENU UI", meta = (BindWidget))	class UUserWidget*		OverMenuUserWidget		= nullptr;

private:
	void UpdateNavigation(const UUserWidget* Widget) const;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UCounterWidget*		GetCounter()	const;
	UHealthBarWidget*   GetHealthBar()	const;

	UMenuWidget*		GetMainMenu()	const;
	UWinWidget*			GetWinMenu()	const;
	UOverWidget*		GetOverMenu()	const;

	UBrawlerInstance*	GetBrawlerInstance() const;
	
	UFUNCTION(BlueprintCallable) void QuitGameEvent();
	UFUNCTION(BlueprintCallable) void PlayGameEvent();
	
	UFUNCTION(BlueprintCallable) void ShowMenuGameEvent();
	UFUNCTION(BlueprintCallable) void HideMenuGameEvent();
	
	UFUNCTION(BlueprintCallable) void ShowOverMenuEvent();
	UFUNCTION(BlueprintCallable) void HideOverMenuEvent();
	
	UFUNCTION(BlueprintCallable) void ShowWinMenuEvent();
	UFUNCTION(BlueprintCallable) void HideWinMenuEvent();
};
