

#pragma once

#include "CoreMinimal.h"
#include "ButtonWidgetClass.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

UCLASS()
class UNREALBRAWLER_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Pause UI", meta = (BindWidget)) class UUserWidget* ResumeButtonWidget;
	UPROPERTY(EditAnywhere, Category = "Pause UI", meta = (BindWidget)) class UUserWidget* MenuButtonWidget;

	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeInAnimation;
	UPROPERTY(Transient, meta = (BindWidgetAnim)) class UWidgetAnimation* FadeOutAnimation;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UButtonWidget* GetResumeButton() const;
	UButtonWidget* GetMenuButton() const;

	UWidgetAnimation* GetFadeAnimation(FString AnimationName) const;
	
	void PlayFadeIn();
	void PlayFadeOut();
};
