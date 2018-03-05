// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "Weapon/BaseWeapon.h"
#include "CharacterAnimation.h"
#include "HeroCharacter.generated.h"


USTRUCT(Blueprintable)
struct FRPGPlayerInput
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HoldingDefend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HoldingSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool AttackPressed;
};

USTRUCT(Blueprintable)
struct FRPGEquiptItems
{
	GENERATED_BODY()
};

USTRUCT(Blueprintable)
struct FRPGDebugFlags
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool DrawUsableLineTrace;
};

UENUM(Blueprintable)
enum class ECountdownTimerZone : uint8
{
	CTZ_Normal,
	CTZ_High,
	CTZ_Critical
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerScoreUpdate, int, DeltaScore);

/**
 * 
 */
UCLASS()
class SWORDMAZE_API AHeroCharacter : public ABaseCharacter, public ICharacterAnimation
{
	GENERATED_BODY()
	
public:
	AHeroCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void GetCharacterInfo(FCharacterAnimationInfo& animInfo);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void AttackStartEnd(bool IsAttacking);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Player)
	void EndAnimInfo(bool IsFreeToAnimate, bool LockRotation);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void AddWeapon(class ABaseWeapon* Weapon);

	/** Getter and setters			*/
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category = Player)
	FORCEINLINE bool IsWeaponEquiped() const { return (EquipedWeapon != nullptr); };

	UFUNCTION(BlueprintCallable, Category = Player)
	FName GetInventoryAttachPoint(EItemType const& slot);

	UPROPERTY(BlueprintAssignable, Category = PickupEvent)
	FOnPlayerScoreUpdate UpdatePlayerScore;

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void TraceWeapon();

	/** Inputs					*/
protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to turn look up/down at a given rate.
	* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void LookUpAtRate(float Rate);

	/** Called when Player wants to Sprint				*/
	void OnStartSprinting();
	void OnStopSprinting();

	/** Player starts defending							*/
	void OnStartDefending();
	void OnStopDefending();

	/** Player wants to attack							*/
	void OnStartAttacking();
	void OnStopAttacking();

	UFUNCTION()
	void OnEquipTimerEnd();

	UFUNCTION(BlueprintNativeEvent, Category = Player)
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						 bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = Player)
	bool UpdateCurrentTimerZone();

	UFUNCTION(BlueprintCallable, Category = Player)
	float GetZoneTimeDelay(ECountdownTimerZone const& TimerZone);

	UFUNCTION(BlueprintCallable, Category = Player)
	void PlayCurrSoundCue();

	/** Components				*/
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player)
	class UBoxComponent* PickupCollector;

protected:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	TMap<ECountdownTimerZone, class USoundCue*> SoundMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Player)
	float WeaponEquipTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	FRPGPlayerInput PlayerInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	FRPGDebugFlags DebugFlags;

	UPROPERTY(Transient)
	class ABaseWeapon* EquipedWeapon;

private:
	UFUNCTION(BlueprintCallable, Category = Player)
	void PlayAttackAnim();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FName PrimaryWeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FName SecondaryWeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equiptment")
	FName HeadArmourSocketName;

	UPROPERTY()
	FTimerHandle WeaponEquipedTimeHandler;

	UPROPERTY()
	FTimerHandle AttackTimeHandler;

	UPROPERTY()
	float ElapsedTime;
	
	UPROPERTY()
	ECountdownTimerZone CurrTimerZone;

	bool bHasNewFocus;	
};
