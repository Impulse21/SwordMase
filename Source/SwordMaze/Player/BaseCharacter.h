// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon/BaseWeapon.h"
#include "CharacterAnimation.h"
#include "BaseCharacter.generated.h"

USTRUCT(Blueprintable)
struct FAnimMontageList
{
	GENERATED_BODY()

	UPROPERTY(transient)
	uint8 CurrAnimIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = CharacterAnim)
	TArray<class UAnimMontage*> AttackAnimations;
};

UCLASS()
class SWORDMAZE_API ABaseCharacter : public ACharacter, public ICharacterAnimation
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = BaseCharacter)
	FORCEINLINE bool IsDead() const { return bIsDead; };

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
	void CalculateHealth(float delta);

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
	void CalculateDead();

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
	void OnDeath(AActor* DamageCauser, FDamageEvent const& DamageEvent);

	UFUNCTION(BlueprintPure, Category = BaseCharacter)
	FORCEINLINE  bool IsAttacking() const { return Attacking; };

	UFUNCTION(BlueprintPure, Category = BaseCharacter)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; };

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = BaseCharacter)
	void GetCharacterInfo(FCharacterAnimationInfo& animInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void AttackStartEnd(bool IsAttacking);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void EndAnimInfo(bool IsFreeToAnimate, bool LockRotation);

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetSprinting(bool sprinting);

	virtual bool IsSprinting();

	virtual void ToggleDefend(bool defend);

protected:
	UFUNCTION(BlueprintCallable, Category = Player)
	void PlayAttackAnim();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseChatacter)
	FAnimMontageList AttackAnims;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BaseChatacter)
	bool Attacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BaseCharacter)
	bool Sprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BaseCharacter)
	bool Blocking;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BaseCharacter)
	bool IsUsingMagic;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = BaseCharacter)
	bool FreeToAnimate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseCharacter)
	float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseCharacter)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = BaseCharacter)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BaseCharacter)
	class UAnimMontage* DeathAnimation;

private:
	UPROPERTY(Transient)
	float CurrentHealth;

	bool bIsDead;
};
	