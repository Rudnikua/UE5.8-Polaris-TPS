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
class UGameplayAbility;

UCLASS(Abstract)
class POLARIS_API APolarisBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APolarisBaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	FORCEINLINE UPolarisAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartAiming();
	void StopAiming();

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat")
	bool IsAiming() const;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polaris|Input")
	TObjectPtr<UInputAction> AimAction;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|GAS", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPolarisAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Polaris|GAS")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UFUNCTION(BlueprintPure, Category = "Polaris|Combat")
	FRotator GetAimOffsets() const;
};
