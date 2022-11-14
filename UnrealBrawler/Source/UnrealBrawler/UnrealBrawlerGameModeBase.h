#pragma once

#include "CoreMinimal.h"
#include "GameHUD.h"
#include "KnifeActor.h"
#include "GameFramework/GameModeBase.h"
#include "UnrealBrawlerGameModeBase.generated.h"

UCLASS()
class UNREALBRAWLER_API AUnrealBrawlerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

private:
	UGameHUD* GameHUD = nullptr;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UserInterface", meta = (AllowPrivateAccess = "true")) TSubclassOf<UUserWidget> WidgetGameClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UserInterface", meta = (AllowPrivateAccess = "true")) TSubclassOf<UUserWidget> WidgetHomeClass;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	UGameHUD* GetGameHUD();
};
