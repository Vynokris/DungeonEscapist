

#pragma once

#include "CoreMinimal.h"
#include "CounterWidget.h"
#include "HealthBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

UCLASS()
class UNREALBRAWLER_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "HUD UI", meta = (BindWidget)) class UUserWidget* CounterWidget;
	UPROPERTY(EditAnywhere, Category = "HUD UI", meta = (BindWidget)) class UUserWidget* HealthBarWidget;
	
protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UCounterWidget* GetCounter() const;
	UHealthBarWidget* GetHealthBar() const;
};
