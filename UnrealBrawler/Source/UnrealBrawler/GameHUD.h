

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

UCLASS()
class UNREALBRAWLER_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Game UI", meta = (BindWidget)) class UTextBlock* EnemyCounter;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable) void UpdateCounterEvent(const FString& Amount);
};
