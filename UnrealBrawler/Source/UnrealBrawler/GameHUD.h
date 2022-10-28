

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

UCLASS()
class UNREALBRAWLER_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UFUNCTION() FText SetCounterText(int _Value) const;
	
protected:
	virtual void NativeConstruct() override;
	virtual bool Initialize() override;
	
public:
	void UpdateEnemyCounterEvent(int _Value);

public:
	UPROPERTY (EditAnywhere, Category = "Game UI", meta = (BindWidget)) class UTextBlock* EnemyCounter;
};
