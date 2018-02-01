// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class SWORDMAZE_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UFUNCTION(BlueprintPure, Category = BaseCharacter)
	FORCEINLINE bool IsDead() const { return bIsDead; };

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
	void CalculateHealth(float delta);

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
	void CalculateDead();

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
	void OnDeath(AActor* DamageCauser, FDamageEvent const& DamageEvent);

	UFUNCTION(BlueprintPure, Category = BaseCharacter)
	FORCEINLINE bool IsAttacking() const { return Attacking; };

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	virtual void SetSprinting(bool sprinting);

	virtual bool IsSprinting();

	virtual void ToggleDefend(bool defend);

protected:
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
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = BaseCharacter)
	class UAnimMontage* DeathAnimation;

private:
	bool bIsDead;
};
	