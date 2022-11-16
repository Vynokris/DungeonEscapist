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
    UUserWidget*				 CurrentHUD					= nullptr;
    
    UPROPERTY(EditAnywhere, Category = "Camera") float CameraLag      = 15.f;
    UPROPERTY(EditAnywhere, Category = "Camera") float CameraDistance = 300.f;
    
    UPROPERTY(EditAnywhere, Category = "Character") int PlayerMaxHealth       = 5;
    UPROPERTY(EditAnywhere, Category = "Character") int EnemyMaxHealth        = 3;
    UPROPERTY(EditAnywhere, Category = "Character") int AttackDamage          = 1;
    UPROPERTY(EditAnywhere, Category = "Character") int InvincibilityDuration = 2;
    
    UPROPERTY(EditDefaultsOnly, Category = "Character") TArray<TSubclassOf<AEquipmentActor>> PlayerDefaultEquipment;
    UPROPERTY(EditDefaultsOnly, Category = "Character") TArray<TSubclassOf<AEquipmentActor>> EnemyDefaultEquipment;
    
    UPROPERTY(EditAnywhere, Category = "VFX") UParticleSystemComponent* ParticleSystemComponent = nullptr;
    UPROPERTY(EditAnywhere, Category = "VFX") float WalkingFxRate = 0.2f;
    UPROPERTY(EditAnywhere, Category = "VFX") UNiagaraSystem* BloodSplatterEffect = nullptr;

    UPROPERTY(EditAnywhere, Category = "GameMode") AUnrealBrawlerGameModeBase* BrawlerGameMode;
    
    UPROPERTY(EditAnywhere, Category = "Character Movement: Walking") float MaxEnemyWalkSpeed = 500;

    bool  CharacterIsPlayer  = true;
    int   Health             = 0;
    int   KillCount          = 0;
    bool  Attacking          = false;
    bool  AttackBlocked      = false;
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
    UFUNCTION(BlueprintCallable) void AttackBlockedEvent();
    UFUNCTION(BlueprintCallable) void StartDefendingEvent();
    UFUNCTION(BlueprintCallable) void StopDefendingEvent();
    UFUNCTION(BlueprintCallable) void StartInvincibilityEvent();
    UFUNCTION(BlueprintCallable) void DeathEvent();
    UFUNCTION(BlueprintCallable) void EnemyKilledEvent();
    UFUNCTION(BlueprintCallable) void DropEquipmentEvent(const EEquipmentType& EquipmentType);
    UFUNCTION(BlueprintCallable) void PickupEquipmentEvent(AEquipmentActor* NewEquipment);

    UFUNCTION(BlueprintCallable) int  IsPlayer()	     const;
    UFUNCTION(BlueprintCallable) int  IsEnemy()		     const;
    UFUNCTION(BlueprintCallable) int  GetHealth()	     const;
    UFUNCTION(BlueprintCallable) int  GetAttackDamage()  const;
    UFUNCTION(BlueprintCallable) int  GetKillCount()     const;
    UFUNCTION(BlueprintCallable) bool IsAttacking()	     const;
    UFUNCTION(BlueprintCallable) bool WasAttackBlocked() const;
    UFUNCTION(BlueprintCallable) bool IsDefending()	     const;
    UFUNCTION(BlueprintCallable) bool IsInvincible()     const;
    UFUNCTION(BlueprintCallable) bool IsDead()           const;
    UFUNCTION(BlueprintCallable) bool HasEquipment(const EEquipmentType& EquipmentType) const;
};
