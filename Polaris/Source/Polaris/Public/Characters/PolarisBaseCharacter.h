// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"
#include "PolarisTypes.h"
#include "PolarisBaseCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UAbilitySystemComponent;
class UPolarisAttributeSet;
class UPolarisCombatAttributeSet;
class UGameplayAbility;
class APolarisWeapon;

UCLASS(Abstract)
class POLARIS_API APolarisBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APolarisBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UPolarisAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE UPolarisCombatAttributeSet* GetCombatAttributeSet() const { return CombatAttributeSet; }

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat", meta = (BlueprintThreadSafe))
	APolarisWeapon* GetCurrentWeapon() const { return CurrentWeapon; }

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat|Weapon", meta = (BlueprintThreadSafe))
	bool IsWeaponEquipped() const { return CurrentWeapon != nullptr && bIsWeaponEquipped && !bIsEquipping; }

	UFUNCTION(BlueprintCallable, Category = "Polaris|Combat|Weapon")
	void AttachWeaponToHand();

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat|Weapon", meta = (BlueprintThreadSafe))
	bool ShouldUseLeftHandIK() const;

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat|Weapon", meta = (BlueprintThreadSafe))
	FTransform GetLeftHandIKTransformCS() const;
protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartAiming();
	void StopAiming();
	void StartFiring();
	void StopFiring();

	void SpawnDefaultWeapon();
	void ToggleEquipWeapon();
	void EquipWeapon();
	void UnequipWeapon();
	void OnEquipMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void UpdateWallCollisionTrace(float DeltaTime);

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat")
	bool IsAiming() const;

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat")
	bool IsWeaponBlocked() const;

	void GiveStartupAbilities();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Camera")
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Enhanced Input Context & Actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> EquipAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Combat")
	EPolarisOverlayState OverlayState = EPolarisOverlayState::Unarmed;

	/* adjust ADS*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	float DefaultFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	float AimFOV = 65.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	float DefaultArmLength = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	float AimArmLength = 160.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	FVector DefaultSocketOffset = FVector(0.0f, 50.0f, 50.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	FVector AimSocketOffset = FVector(0.0f, 75.0f, 65.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Polaris|Camera|ADS")
	float CameraInterpSpeed = 12.0f;
	/* adjust ADS\*/


	/* adjust Wall Trace */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Combat")
	float WallProximityAlpha = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|WallTrace")
	float WeaponTraceDistance = 110.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|WallTrace")
	float WeaponTraceRadius = 12.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|WallTrace")
	float WeaponTraceHeightOffset = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|WallTrace")
	float WeaponTraceRightOffset = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Polaris|Combat|WallTrace")
	bool bDrawDebugWallTrace = false;
	/* adjust Wall Trace\*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPolarisAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|GAS")
	TObjectPtr<UPolarisCombatAttributeSet> CombatAttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polaris|GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat")
	FRotator GetAimOffsets() const;

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|Weapon")
	TSubclassOf<APolarisWeapon> DefaultWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|Weapon")
	FName WeaponAttachSocketName = FName("WeaponSocket_R");

	UPROPERTY(EditDefaultsOnly, Category = "Polaris|Combat|Weapon")
	FName WeaponHolsterSocketName = FName("WeaponSocket_Back");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Combat|Weapon")
	TObjectPtr<APolarisWeapon> CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Combat|Weapon")
	bool bIsWeaponEquipped = false;

	/* Animation montages */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Combat|Weapon")
	TObjectPtr<UAnimMontage> EquipAnimMontage;
	/* Animation montages\*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Combat|Weapon")
	bool bIsEquipping = false;
};
