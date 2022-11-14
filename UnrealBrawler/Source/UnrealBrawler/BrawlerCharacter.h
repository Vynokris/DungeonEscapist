#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameHUD.h"
#include "EquipmentActor.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "NiagaraComponent.h"
#include "UnrealBrawlerGameModeBase.h"
#include "BrawlerCharacter.generated.h"

UCLASS()
class UNREALBRAWLER_API ABrawlerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UCameraComponent*            CameraComponent            = nullptr;
	USpringArmComponent*         SpringArmComponent         = nullptr;
	UCharacterMovementComponent* CharacterMovementComponent = nullptr;
	AAIController*				 AiController				= nullptr;
	UUserWidget*				 CurrentHUD					= nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true")) float CameraLag      = 15.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true")) float CameraDistance = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int PlayerMaxHealth       = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int EnemyMaxHealth        = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int AttackDamage          = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) int InvincibilityDuration = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) TSubclassOf<AActor> PlayerDefaultWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true")) TSubclassOf<AActor> EnemyDefaultWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (AllowPrivateAccess = "true")) UShapeComponent* ReachComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true")) UParticleSystemComponent* ParticleSystemComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true")) float WalkingFxRate = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX", meta = (AllowPrivateAccess = "true")) UNiagaraSystem* BloodSplatterEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameMode", meta = (AllowPrivateAccess = "true")) TSubclassOf<AGameModeBase> GameMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Movement: Walking", meta = (AllowPrivateAccess = "true")) float MaxEnemyWalkSpeed = 500;

	bool  CharacterIsPlayer  = true;
	int   Health             = 0;
	int   KillCount          = 0;
	bool  Attacking          = false;
	bool  Defending          = false;
	float InvincibilityTimer = 0;

	TArray<AEquipmentActor*> Equipment;
	
private:
	void MoveForward(const float Amount);
	void MoveRight  (const float Amount);

	void UpdateWalkingFX() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(UInputComponent* NewInputComponent) override;
	
public:
	ABrawlerCharacter();
	
	UFUNCTION(BlueprintCallable) void TakeDamageEvent(const int& Amount);
	UFUNCTION(BlueprintCallable) void StartAttackingEvent();
	UFUNCTION(BlueprintCallable) void StopAttackingEvent();
	UFUNCTION(BlueprintCallable) void StartDefendingEvent();
	UFUNCTION(BlueprintCallable) void StopDefendingEvent();
	UFUNCTION(BlueprintCallable) void StartInvincibilityEvent();
	UFUNCTION(BlueprintCallable) void DeathEvent();
	UFUNCTION(BlueprintCallable) void EnemyKilledEvent();
	UFUNCTION(BlueprintCallable) void DropEquipmentEvent(const EEquipmentType& EquipmentType);
	UFUNCTION(BlueprintCallable) void PickupEquipmentEvent(AEquipmentActor* NewEquipment);

	UFUNCTION(BlueprintCallable) int  IsPlayer()	    const;
	UFUNCTION(BlueprintCallable) int  IsEnemy()		    const;
	UFUNCTION(BlueprintCallable) int  GetHealth()	    const;
	UFUNCTION(BlueprintCallable) int  GetAttackDamage() const;
	UFUNCTION(BlueprintCallable) int  GetKillCount()    const;
	UFUNCTION(BlueprintCallable) bool IsAttacking()	    const;
	UFUNCTION(BlueprintCallable) bool IsDefending()	    const;
	UFUNCTION(BlueprintCallable) bool IsInvincible()    const;
	UFUNCTION(BlueprintCallable) bool IsDead()          const;
	UFUNCTION(BlueprintCallable) bool HasEquipment(const EEquipmentType& EquipmentType) const;
};
