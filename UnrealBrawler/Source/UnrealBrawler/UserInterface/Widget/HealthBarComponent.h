

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

UCLASS()
class UNREALBRAWLER_API UHealthBarComponent : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UProgressBar* HealthBar;
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UTextBlock* CurrentHealthText;
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UTextBlock* TotalHealthText;
    UPROPERTY(EditAnywhere, Category = "Health UI", meta = (BindWidget)) class UTextBlock* SeparatorText;
    
protected:
    virtual bool Initialize() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:
    UFUNCTION(BlueprintCallable) void SetupHealthComponent(const AActor* OwnerActor);
    
    UFUNCTION(BlueprintCallable) void UpdateHealthEvent(const float& Amount);
    UFUNCTION(BlueprintCallable) void UpdateTotalHealthTextEvent(const FString& Amount);
    UFUNCTION(BlueprintCallable) void UpdateCurrentHealthTextEvent(const FString& Amount);

    UProgressBar* GetHealthBar() const;
    UTextBlock* GetCurrentHealthText() const;
    UTextBlock* GetTotalHealthText() const;
    UTextBlock* GetSeparatorText() const;
};
