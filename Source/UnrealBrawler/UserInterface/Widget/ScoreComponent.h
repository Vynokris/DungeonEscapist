

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreComponent.generated.h"

UCLASS()
class UNREALBRAWLER_API UScoreComponent : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Score UI", meta = (BindWidget)) class UTextBlock* CurrentScorePoint;

protected:
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable) void SetupScoreComponent(const AActor* Actor);
	
	UFUNCTION(BlueprintCallable) void UpdateScoreEvent(const FText& Amount);
};
