#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameHUD.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "BrawlerPlayer.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UCameraComponent*            CameraComponent            = nullptr;
	USpringArmComponent*         SpringArmComponent         = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	UGameHUD*                    GameHudComponent           = nullptr;
	AAIController*				 AiController				= nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true")) float CameraLag      = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true")) float CameraDistance = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int Health                = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int AttackDamage          = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int Stamina               = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int AttackDuration        = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int InvincibilityDuration = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true")) UParticleSystemComponent* ParticleSystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true")) float WalkingFxRate = 0.2f;
	
	int   KillCount          = 0;
	float AttackTimer        = 0;
	bool  Defending          = false;
	float InvincibilityTimer = 0;
	
private:
	void MoveForward(const float Amount);
	void MoveRight  (const float Amount);

	float GetMoveForward() const { return 0; } // TODO: Find a way to delete these without crashing Unreal.
	float GetMoveRight()   const { return 0; } // TODO: Find a way to delete these without crashing Unreal.

	void UpdateWalkingFX() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* NewInputComponent) override;
	
public:
	ABrawlerPlayer();

	UFUNCTION(BlueprintCallable) int  GetHealth()    const;
	UFUNCTION(BlueprintCallable) int  GetStamina()   const;
	UFUNCTION(BlueprintCallable) int  GetKillCount() const;
	UFUNCTION(BlueprintCallable) bool IsAttacking()  const;
	UFUNCTION(BlueprintCallable) bool IsDefending()  const;
	UFUNCTION(BlueprintCallable) bool IsInvincible() const;
	UFUNCTION(BlueprintCallable) bool IsDead()       const;
	
	void TakeDamageEvent(const int& Amount);
	void AttackEvent();
	void StartDefendingEvent();
	void StopDefendingEvent();
	void InvincibilityEvent();
	void DeathEvent();
	void EnemyKilledEvent();
};
