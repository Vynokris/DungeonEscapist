

#pragma once

#include "CoreMinimal.h"
#include "CounterWidgetClass.h"
#include "HealthBarWidgetClass.h"
#include "MenuWidgetClass.h"
#include "OverWidgetClass.h"
#include "WinWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "UserInterfaceManager.generated.h"

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
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UCounterWidget* GetCounter() const;
	UHealthBarWidget* GetHealthBar() const;

	UFUNCTION(BlueprintCallable) void QuitGameEvent();
	UFUNCTION(BlueprintCallable) void PlayGameEvent();
	UFUNCTION(BlueprintCallable) void RestartGameEvent();
	
	UFUNCTION(BlueprintCallable) void ShowMenuGameEvent();
	UFUNCTION(BlueprintCallable) void ShowOverMenuEvent();
	UFUNCTION(BlueprintCallable) void ShowWinMenuEvent();

private:
	
};
