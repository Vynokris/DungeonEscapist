

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KillCOunterComponent.generated.h"

UCLASS()
class UNREALBRAWLER_API UKillCounterComponent : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Counter UI", meta = (BindWidget)) class UTextBlock* CurrentEnemyKilled;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable) void SetupCounterComponent(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable) void UpdateCounterEvent(const FText& Amount);
};
