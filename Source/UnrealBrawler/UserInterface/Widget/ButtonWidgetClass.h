

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidgetClass.generated.h"

UCLASS()
class UNREALBRAWLER_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Bouton UI", meta = (BindWidget)) class UTextBlock* DisplayedText;
	UPROPERTY(EditAnywhere, Category = "Bouton UI", meta = (BindWidget)) class UButton* Button;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetText(const FText& Value) const;
	UButton* GetButton() const;
};
