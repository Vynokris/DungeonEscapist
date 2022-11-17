

#pragma once

#include "CoreMinimal.h"
#include "ButtonWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "OverWidgetClass.generated.h"

UCLASS()
class UNREALBRAWLER_API UOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* RestartButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* QuitButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* MenuButtonWidget;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	//UButtonWidget* GetRestartButton() const;
	UButtonWidget* GetQuitButton() const;
	UButtonWidget* GetMenuButton() const;
};
