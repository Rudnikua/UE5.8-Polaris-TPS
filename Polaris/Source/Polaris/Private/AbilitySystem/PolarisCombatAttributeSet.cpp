#include "AbilitySystem/PolarisCombatAttributeSet.h"
#include "GameplayEffectExtension.h"

UPolarisCombatAttributeSet::UPolarisCombatAttributeSet()
{
	// Initialize default values for Assault Rifle attributes
	InitMagazineAmmo(30.0f);
	InitMaxMagazineAmmo(30.0f);

	InitReserveAmmo(120.0f);
	InitMaxReserveAmmo(120.0f);

	InitWeaponSpread(0.0f);
	InitBaseDamage(25.0f);
}

void UPolarisCombatAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetMagazineAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMagazineAmmo());
	}
	else if (Attribute == GetReserveAmmoAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxReserveAmmo());
	}
	else if (Attribute == GetWeaponSpreadAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UPolarisCombatAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	const FGameplayAttribute& ModifiedAttribute = Data.EvaluatedData.Attribute;

	if (ModifiedAttribute == GetMagazineAmmoAttribute())
	{
		SetMagazineAmmo(FMath::Clamp(GetMagazineAmmo(), 0.0f, GetMaxMagazineAmmo()));
	}
	else if (ModifiedAttribute == GetReserveAmmoAttribute())
	{
		SetReserveAmmo(FMath::Clamp(GetReserveAmmo(), 0.0f, GetMaxReserveAmmo()));
	}
	else if (ModifiedAttribute == GetWeaponSpreadAttribute())
	{
		SetWeaponSpread(FMath::Max(GetWeaponSpread(), 0.0f));
	}
}