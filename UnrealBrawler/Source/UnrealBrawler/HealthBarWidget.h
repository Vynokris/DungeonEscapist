

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

UCLASS()
class UNREALBRAWLER_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UProgressBar* HealthBar;
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UTextBlock* CurrentHealthText;
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UTextBlock* TotalHealthText;

protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:
    UFUNCTION(BlueprintCallable) void UpdateHealthEvent(const int& Amount);
    UFUNCTION(BlueprintCallable) void UpdateTotalHealthTextEvent(const FString& Amount);
    UFUNCTION(BlueprintCallable) void UpdateCurrentHealthTextEvent(const FString& Amount);
};
