

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
	UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* QuitButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Over UI", meta = (BindWidget)) class UUserWidget* MenuButtonWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeOutAnimation;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UButtonWidget* GetQuitButton() const;
	UButtonWidget* GetMenuButton() const;

	UWidgetAnimation* GetFadeAnimation(FString AnimationName) const;
	
	void PlayFadeIn();
	void PlayFadeOut();
};
