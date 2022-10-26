#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BrawlerNpc.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerNpc : public ACharacter
{
	GENERATED_BODY()

protected:
	// The direction in which the AI strafes when the player isn't visible (1 : right, -1 : left).
	int StrafeDir;
	
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Sets default values for this character's properties.
	ABrawlerNpc();
	
	// Called every frame.
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Returns the AI's strafe direction (1 : right, -1 : left).
	int GetStrafeDir() const;
};
