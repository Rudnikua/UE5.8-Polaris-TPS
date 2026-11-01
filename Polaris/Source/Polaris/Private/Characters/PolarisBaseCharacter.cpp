// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PolarisBaseCharacter.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/PolarisAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/PolarisGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

APolarisBaseCharacter::APolarisBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultArmLength;
	CameraBoom->SocketOffset = DefaultSocketOffset;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UPolarisAttributeSet>(TEXT("AttributeSet"));
}

void APolarisBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CameraBoom && FollowCamera)
	{
		const bool bCurrentlyAiming = IsAiming() && !IsWeaponBlocked();
		const float TargetFOV = bCurrentlyAiming ? AimFOV : DefaultFOV;
		const float TargetArmLength = bCurrentlyAiming ? AimArmLength : DefaultArmLength;
		const FVector TargetSocketOffset = bCurrentlyAiming ? AimSocketOffset : DefaultSocketOffset;

		FollowCamera->SetFieldOfView(FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, CameraInterpSpeed));
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, TargetArmLength, DeltaTime, CameraInterpSpeed);
		CameraBoom->SocketOffset = FMath::VInterpTo(CameraBoom->SocketOffset, TargetSocketOffset, DeltaTime, CameraInterpSpeed);
	}

	UpdateWallCollisionTrace(DeltaTime);
}

UAbilitySystemComponent* APolarisBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void APolarisBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (const APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

void APolarisBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		GiveStartupAbilities();
	}
}

void APolarisBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APolarisBaseCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APolarisBaseCharacter::Look);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APolarisBaseCharacter::StartAiming);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APolarisBaseCharacter::StopAiming);
	}
}

void APolarisBaseCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APolarisBaseCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APolarisBaseCharacter::StartAiming()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(PolarisGameplayTags::State_Aiming, 1);
	}

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 350.0f; 
}

void APolarisBaseCharacter::StopAiming()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(PolarisGameplayTags::State_Aiming, 0);
	}

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f; 
}

void APolarisBaseCharacter::UpdateWallCollisionTrace(float DeltaTime)
{
	if (OverlayState == EPolarisOverlayState::Unarmed)
	{
		WallProximityAlpha = FMath::FInterpTo(WallProximityAlpha, 0.0f, DeltaTime, 12.0f);
		if (AbilitySystemComponent && IsWeaponBlocked())
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(PolarisGameplayTags::State_WeaponBlocked, 0);
		}
		return;
	}

	const FVector StartLocation = GetActorLocation() + 
		(GetActorUpVector() * WeaponTraceHeightOffset) +
		(GetActorRightVector() * WeaponTraceRightOffset);

	FRotator TraceRotation = GetActorRotation();
	if (IsAiming())
	{
		TraceRotation.Pitch = FMath::ClampAngle(GetControlRotation().Pitch, -45.0f, 45.0f);
	}

	const FVector EndLocation = StartLocation + (TraceRotation.Vector() * WeaponTraceDistance);
		
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	const EDrawDebugTrace::Type DrawDebugType = bDrawDebugWallTrace
		? EDrawDebugTrace::ForOneFrame
		: EDrawDebugTrace::None;

	FHitResult HitResult;
	const bool bHit = UKismetSystemLibrary::SphereTraceSingle(
		this,
		StartLocation,
		EndLocation,
		WeaponTraceRadius,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, // bTraceComplex
		ActorsToIgnore,
		DrawDebugType,
		HitResult,
		true, // bIgnoreSelf
		FLinearColor::Green, 
		FLinearColor::Red    
	);

	float TargetAlpha = 0.0f;

	if (bHit && HitResult.bBlockingHit)
	{
		const float SurfaceDot = FVector::DotProduct(HitResult.ImpactNormal, FVector::UpVector);
		if (SurfaceDot < 0.7f)
		{
			const float MinWallDistance = 25.0f;
			TargetAlpha = FMath::GetMappedRangeValueClamped(
				FVector2D(MinWallDistance, WeaponTraceDistance),
				FVector2D(1.0f, 0.0f),
				HitResult.Distance
			);
		}
	}

	WallProximityAlpha = FMath::FInterpTo(WallProximityAlpha, TargetAlpha, DeltaTime, 15.0f);

	const bool bShouldBlock = WallProximityAlpha > 0.35f;
	if (AbilitySystemComponent)
	{
		const bool bCurrentlyBlocked = IsWeaponBlocked();
		if (bShouldBlock != bCurrentlyBlocked)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(PolarisGameplayTags::State_WeaponBlocked, bShouldBlock ? 1 : 0);
		}
	}
}

bool APolarisBaseCharacter::IsAiming() const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(PolarisGameplayTags::State_Aiming);
	}
	return false;
}

bool APolarisBaseCharacter::IsWeaponBlocked() const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(PolarisGameplayTags::State_WeaponBlocked);
	}
	return false;
}


void APolarisBaseCharacter::GiveStartupAbilities()
{
	if (!AbilitySystemComponent) return;

	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
	{
		if (AbilityClass)
		{
			FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

FRotator APolarisBaseCharacter::GetAimOffsets() const
{
	const FRotator ControlRotation = GetControlRotation();
	const FRotator ActorRotation = GetActorRotation();

	FRotator DeltaRotation = (ControlRotation - ActorRotation).GetNormalized();
	return DeltaRotation;
}
