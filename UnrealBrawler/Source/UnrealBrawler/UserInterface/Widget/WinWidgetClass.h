

#pragma once

#include "CoreMinimal.h"
#include "ButtonWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "WinWidgetClass.generated.h"

UCLASS()
class UNREALBRAWLER_API UWinWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, Category = "Win UI", meta = (BindWidget)) class UUserWidget* RestartButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Win UI", meta = (BindWidget)) class UUserWidget* MenuButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Win UI", meta = (BindWidget)) class UUserWidget* QuitButtonWidget;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	//UButtonWidget* GetRestartButton() const;
	UButtonWidget* GetMenuButton() const;
	UButtonWidget* GetQuitButton() const;
};
