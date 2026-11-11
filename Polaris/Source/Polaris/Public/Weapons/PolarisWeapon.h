#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PolarisWeapon.generated.h"

UENUM(BlueprintType)
enum class EPolarisFireMode : uint8
{
	Single		UMETA(DisplayName = "Single (Semi-Auto)"),
	Burst		UMETA(DisplayName = "Burst"),
	FullAuto	UMETA(DisplayName = "Full Auto")
};

UCLASS()
class POLARIS_API APolarisWeapon : public AActor
{
	GENERATED_BODY()

public:
	APolarisWeapon();

	// Повертає світову локацію та ротацію сокета дула (Muzzle)
	FVector GetMuzzleLocation() const;
	FRotator GetMuzzleRotation() const;

	// Повертає світовий трансформ сокета під ліву руку
	UFUNCTION(BlueprintPure, Category = "Polaris|Weapon")
	FTransform GetLeftHandIKTransform() const;

	FORCEINLINE float GetTimeBetweenShots() const { return 60.0f / FMath::Max(1.0f, FireRateRPM); }
	FORCEINLINE EPolarisFireMode GetFireMode() const { return FireMode; }
	FORCEINLINE float GetMaxRange() const { return MaxRange; }
	FORCEINLINE float GetDamageFalloffStart() const { return DamageFalloffStart; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Polaris|Weapon")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Weapon|Sockets")
	FName MuzzleSocketName = FName("Muzzle");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Polaris|Weapon|Sockets")
	FName LeftHandIKSocketName = FName("LeftHandIK");

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Polaris|Weapon|Stats")
	EPolarisFireMode FireMode = EPolarisFireMode::FullAuto;

	// Пострілів за хвилину (для M4A1 стандарт 750-800 RPM -> ~0.08 сек між пострілами)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Polaris|Weapon|Stats")
	float FireRateRPM = 750.0f;

	// Початок падіння шкоди (25 метрів = 2500 см) та максимальна дистанція (100 м = 10000 см)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Polaris|Weapon|Stats")
	float DamageFalloffStart = 2500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Polaris|Weapon|Stats")
	float MaxRange = 10000.0f;
};