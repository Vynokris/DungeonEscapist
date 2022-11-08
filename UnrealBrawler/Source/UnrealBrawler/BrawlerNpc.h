#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BrawlerNpc.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerNpc : public ACharacter
{
	GENERATED_BODY()

	enum class Type
	{
		TANK,
		FIGHTER,
		HEALER
	};
	
private:
	int damageToApply;
	Type type;
	
public:	
	ABrawlerNpc();

	int GetDamage();
	Type GetType();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(const float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
