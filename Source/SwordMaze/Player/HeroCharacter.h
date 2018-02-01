// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/BaseCharacter.h"
#include "CharacterAnimation.h"
#include "HeroCharacter.generated.h"


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemType : uint8
{
	EI_Primary 		UMETA(DisplayName = "Weapon"),
	EI_Secondary 	UMETA(DisplayName = "Secondary/Sheild"),
	EI_Armour		UMETA(DisplayName = "Armour"),
	EI_Consumable	UMETA(DisplayName = "Consumable")
};

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
		void GetCharacterInfo(FCharacterAnimationInfo& animInfo);

	/** Getter and setters			*/
public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = Player)
		FName GetInventoryAttachPoint(EItemType const& slot);

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

	/** Interacts with item								*/
	void OnInteract();

private:
	UFUNCTION(BlueprintCallable, Category = Player)
	void PlayAttackAnim();

	/** Components				*/
protected:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

protected:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(BlueprintReadWrite, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	float MaxUseDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Player)
	FRPGPlayerInput PlayerInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	FRPGDebugFlags DebugFlags;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FName PrimaryWeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	FName SecondaryWeaponSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equiptment")
	FName HeadArmourSocketName;

	FTimerHandle AttackTimeHandler;
	bool bHasNewFocus;	
};
