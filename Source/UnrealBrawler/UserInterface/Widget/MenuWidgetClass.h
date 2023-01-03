

#pragma once

#include "CoreMinimal.h"
#include "ButtonWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidgetClass.generated.h"

UCLASS()
class UNREALBRAWLER_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget)) class UUserWidget* PlayButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Menu UI", meta = (BindWidget)) class UUserWidget* QuitButtonWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeOutAnimation;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UButtonWidget* GetPlayButton() const;
	UButtonWidget* GetQuitButton() const;

	UWidgetAnimation* GetFadeAnimation(FString AnimationName) const;
	
	void PlayFadeIn();
	void PlayFadeOut();
	
};
