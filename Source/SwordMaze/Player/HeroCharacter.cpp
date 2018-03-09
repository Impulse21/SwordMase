// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Pickup/Pickup.h"
#include "BaseCharacterAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/CharacterAnimInstance.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"

AHeroCharacter::AHeroCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create pickup sphere
	PickupCollector = CreateDefaultSubobject<UBoxComponent>(TEXT("Pickup Collector"));
	PickupCollector->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PickupCollector->OnComponentBeginOverlap.AddDynamic(this, &AHeroCharacter::OnPickupOverlap);
	PickupCollector->AttachTo(GetMesh());

	FreeToAnimate = true;
}


void AHeroCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSprinting())
	{
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (PlayerInput.AttackPressed)
	{
		PlayAttackAnim();
	}

	if (Attacking)
	{
		TraceWeapon();
	}
}

void AHeroCharacter::GetCharacterInfo_Implementation(FCharacterAnimationInfo & animInfo)
{
}

void AHeroCharacter::AttackStartEnd_Implementation(bool IsAttacking)
{
	Attacking = IsAttacking;
}

void AHeroCharacter::EndAnimInfo_Implementation(bool IsFreeToAnimate, bool LockRotation)
{
	FreeToAnimate = IsFreeToAnimate;
}

void AHeroCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AHeroCharacter::OnStartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AHeroCharacter::OnStopSprinting);

	PlayerInputComponent->BindAction("Blocking", IE_Pressed, this, &AHeroCharacter::OnStartDefending);
	PlayerInputComponent->BindAction("Blocking", IE_Released, this, &AHeroCharacter::OnStopDefending);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AHeroCharacter::OnStartAttacking);

	PlayerInputComponent->BindAxis("MoveForward", this, &AHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AHeroCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AHeroCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AHeroCharacter::LookUpAtRate);
}

void AHeroCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AHeroCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AHeroCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AHeroCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AHeroCharacter::OnStartAttacking()
{
	if (EquipedWeapon)
	{
		PlayerInput.AttackPressed = true;
		GetWorldTimerManager().SetTimer(AttackTimeHandler, this, &AHeroCharacter::OnStopAttacking, 3.0f, true, 0.0f);
	}
}

void AHeroCharacter::OnStopAttacking()
{
	PlayerInput.AttackPressed = false;
}

/** Called when Player wants to Sprint				*/
void AHeroCharacter::OnStartSprinting()
{
	PlayerInput.HoldingSprint = true;

	SetSprinting(true);
}

void AHeroCharacter::OnStopSprinting()
{
	UE_LOG(LogTemp, Warning, TEXT("StopingSprint"));
	PlayerInput.HoldingSprint = true;

	SetSprinting(false);
}

/** Player starts defending							*/
void AHeroCharacter::OnStartDefending()
{
	PlayerInput.HoldingDefend = true;

	ToggleDefend(true);
}

void AHeroCharacter::OnStopDefending()
{
	PlayerInput.HoldingDefend = true;

	ToggleDefend(false);
}

FName AHeroCharacter::GetInventoryAttachPoint(EItemType const & slot)
{
	switch (slot)
	{
	case EItemType::EI_Primary:
		return PrimaryWeaponSocketName;
	case EItemType::EI_Secondary:
		return SecondaryWeaponSocketName;
	case EItemType::EI_Armour:
		return HeadArmourSocketName;
	default:
		return "";
	}
}

void AHeroCharacter::PlayAttackAnim()
{
	if (!FreeToAnimate)
	{
		UE_LOG(LogTemp, Warning, TEXT("We are not free to animate"));
		return;
	}

	if (AttackAnims.AttackAnimations.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("There are no animations to animate"));
		return;
	}

	uint8 * CurrAnimIndex = &AttackAnims.CurrAnimIndex;
	UAnimMontage* CurrAnim = nullptr;

	CurrAnim = AttackAnims.AttackAnimations[*CurrAnimIndex];

	*CurrAnimIndex += 1;
	if (*CurrAnimIndex >= AttackAnims.AttackAnimations.Num())
	{
		*CurrAnimIndex = 0;
	}


	if (CurrAnim)
	{
		PlayAnimMontage(CurrAnim);
		FreeToAnimate = false;
		FString Name = (CurrAnim) ? *CurrAnim->GetName() : TEXT("");
		UE_LOG(LogTemp, Warning, TEXT("Animation Selected to play is [%s]"), *Name);
	}
}

void AHeroCharacter::OnPickupOverlap_Implementation(UPrimitiveComponent * OverlappedComp, 
													AActor * OtherActor, 
													UPrimitiveComponent * OtherComp, 
													int32 OtherBodyIndex, 
													bool bFromSweep, 
													const FHitResult & SweepResult)
{
	APickup* pickable = Cast<APickup>(OtherActor);

	if (pickable)
	{
		UE_LOG(LogTemp, Warning, TEXT("On Pickup Occured"));
		pickable->OnPickup(this);
		UpdatePlayerScore.Broadcast(pickable->GetScoreValue());
	}
}

void AHeroCharacter::AddWeapon(class ABaseWeapon* Weapon)
{
	if (Weapon)
	{
		if (EquipedWeapon == nullptr)
		{
			EquipedWeapon = Weapon;
			EquipedWeapon->OnEquip(this);
		}

		if (GetWorldTimerManager().IsTimerActive(WeaponEquipedTimeHandler))
		{
			GetWorldTimerManager().ClearTimer(WeaponEquipedTimeHandler);
		}

		ElapsedTime = 0;
		CurrTimerZone = ECountdownTimerZone::CTZ_Normal;
		UE_LOG(LogTemp, Warning, TEXT("Starting Game time for Weapon Equip Time"));
		GetWorldTimerManager().SetTimer(WeaponEquipedTimeHandler, this, &AHeroCharacter::OnEquipTimerEnd, GetZoneTimeDelay(CurrTimerZone), true);
	}
}

void AHeroCharacter::OnEquipTimerEnd()
{
	if (EquipedWeapon == nullptr)
	{
		return;
	}

	bool updatedZone = UpdateCurrentTimerZone();

	if (ElapsedTime >= WeaponEquipTime)
	{
		EquipedWeapon->OnUnEquip();
		EquipedWeapon = nullptr;
		GetWorldTimerManager().ClearTimer(WeaponEquipedTimeHandler);
	}
	else
	{
		PlayCurrSoundCue();
		ElapsedTime += GetWorldTimerManager().GetTimerElapsed(WeaponEquipedTimeHandler);
	}

	if (updatedZone)
	{
		GetWorldTimerManager().ClearTimer(WeaponEquipedTimeHandler);
		GetWorldTimerManager().SetTimer(WeaponEquipedTimeHandler, this, &AHeroCharacter::OnEquipTimerEnd, GetZoneTimeDelay(CurrTimerZone), true);
	}

}


void AHeroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorldTimerManager().ClearAllTimersForObject(this);
}

bool AHeroCharacter::UpdateCurrentTimerZone()
{
	const float PercentageOfTime = (ElapsedTime / WeaponEquipTime) * 100.0f;
	auto LastZone = CurrTimerZone;

	if (PercentageOfTime < 50.0f)
	{
		CurrTimerZone = ECountdownTimerZone::CTZ_Normal;
	}
	else if (PercentageOfTime > 50.0f && PercentageOfTime < 75.0f)
	{
		CurrTimerZone = ECountdownTimerZone::CTZ_High;
	}
	else if (PercentageOfTime > 75.0f)
	{
		CurrTimerZone = ECountdownTimerZone::CTZ_Critical;
	}
	else
	{
		CurrTimerZone = ECountdownTimerZone::CTZ_Normal;
	}

	return (LastZone != CurrTimerZone);
}

void AHeroCharacter::PlayCurrSoundCue()
{
	if (!SoundMap.Contains(CurrTimerZone))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to locate sound file to play"));
		return;
	}

	auto Sound = SoundMap[CurrTimerZone];

	if (Sound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
	}
}

float AHeroCharacter::GetZoneTimeDelay(ECountdownTimerZone const& TimerZone)
{
	switch (TimerZone)
	{
	case ECountdownTimerZone::CTZ_Normal:
		return 1.0f;
	case ECountdownTimerZone::CTZ_High:
		return 0.5f;
	case ECountdownTimerZone::CTZ_Critical:
		return 0.25f;
	default:
		return 1.0f;
	}
}

void AHeroCharacter::TraceWeapon()
{
	if (EquipedWeapon == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is not equiped"));
		return;
	}

	UStaticMeshComponent* WeaponMesh = EquipedWeapon->GetMesh();

	FVector TraceStart = WeaponMesh->GetSocketLocation("TraceStart");
	FVector TraceEnd = WeaponMesh->GetSocketLocation("TraceEnd");

	FCollisionQueryParams TraceWeaponParams = FCollisionQueryParams(FName(TEXT("TraceWeaponParams")), true, this);
	TraceWeaponParams.bTraceComplex = true;
	TraceWeaponParams.bTraceAsyncScene = true;
	TraceWeaponParams.bReturnPhysicalMaterial = false;

	/** Debug */
	if (DebugFlags.DrawUsableLineTrace)
	{
		const FName TraceTag("TraceWeaponParams");
		GetWorld()->DebugDrawTraceTag = TraceTag;

		TraceWeaponParams.TraceTag = TraceTag;
	}
	/** End Debug  */

	TArray<FHitResult> HitResults;
	GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECC_GameTraceChannel14, TraceWeaponParams);

	for (auto& HitResult : HitResults)
	{
		if (HitResult.Actor->IsA<ABaseCharacter>())
		{
			UGameplayStatics::ApplyDamage(HitResult.Actor.Get(), EquipedWeapon->GetSwordDamage(), this->GetController(), this, EquipedWeapon->GetDamageType());
		}
	}
}