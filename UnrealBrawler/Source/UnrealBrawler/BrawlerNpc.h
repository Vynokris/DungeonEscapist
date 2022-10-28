#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BrawlerNpc.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerNpc : public ACharacter
{
	GENERATED_BODY()

public:	
	// Sets default values for this character's properties.
	ABrawlerNpc();
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;
	
	// Called every frame.
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
