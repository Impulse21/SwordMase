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
#include "TimerManager.h"

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

}

void AHeroCharacter::GetCharacterInfo_Implementation(FCharacterAnimationInfo & animInfo)
{
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
	PlayerInput.AttackPressed = true;
	UE_LOG(LogTemp, Warning, TEXT("Attacking"));
	GetWorldTimerManager().SetTimer(AttackTimeHandler, this, &AHeroCharacter::OnStopAttacking, 3.0f, true, 0.0f);
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
	UBaseCharacterAnimInstance* AnimInstance = Cast<UBaseCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance && !AnimInstance->IsPlayingAttackAnim())
	{
		AnimInstance->Attack();
	}
}

void AHeroCharacter::OnPickupOverlap_Implementation(UPrimitiveComponent * OverlappedComp, 
													AActor * OtherActor, 
													UPrimitiveComponent * OtherComp, 
													int32 OtherBodyIndex, 
													bool bFromSweep, 
													const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("On Pickup Occured"));

	APickup* pickable = Cast<APickup>(OtherActor);

	if (pickable)
	{
		pickable->OnPickup(this);
		UpdatePlayerScore.Broadcast(pickable->GetScoreValue());
	}
}