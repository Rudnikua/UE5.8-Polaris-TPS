#include "Weapons/PolarisWeapon.h"
#include "Components/StaticMeshComponent.h"

APolarisWeapon::APolarisWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);

	// Вимикаємо колізію для мешу зброї, щоб вона не штовхала капсулу гравця і не блокувала камеру
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

FVector APolarisWeapon::GetMuzzleLocation() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		return WeaponMesh->GetSocketLocation(MuzzleSocketName);
	}
	return GetActorLocation();
}

FRotator APolarisWeapon::GetMuzzleRotation() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(MuzzleSocketName))
	{
		return WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	return GetActorRotation();
}

FTransform APolarisWeapon::GetLeftHandIKTransform() const
{
	if (WeaponMesh && WeaponMesh->DoesSocketExist(LeftHandIKSocketName))
	{
		return WeaponMesh->GetSocketTransform(LeftHandIKSocketName, RTS_Component);
	}
	return GetActorTransform();
}