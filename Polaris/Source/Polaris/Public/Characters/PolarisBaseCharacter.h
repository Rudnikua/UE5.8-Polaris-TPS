// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PolarisBaseCharacter.generated.h"

class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;

UCLASS(Abstract)
class POLARIS_API APolarisBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APolarisBaseCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

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
};
