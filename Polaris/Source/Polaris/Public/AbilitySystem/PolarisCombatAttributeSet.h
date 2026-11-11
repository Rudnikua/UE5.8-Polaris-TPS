#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "PolarisCombatAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class POLARIS_API UPolarisCombatAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UPolarisCombatAttributeSet();

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// Ammo & Magazine (GA_FireWeapon, GA_Reload)
	UPROPERTY(BlueprintReadOnly, Category = "Polaris|Attributes|Combat")
	FGameplayAttributeData MagazineAmmo;
	ATTRIBUTE_ACCESSORS(UPolarisCombatAttributeSet, MagazineAmmo)

		UPROPERTY(BlueprintReadOnly, Category = "Polaris|Attributes|Combat")
	FGameplayAttributeData MaxMagazineAmmo;
	ATTRIBUTE_ACCESSORS(UPolarisCombatAttributeSet, MaxMagazineAmmo)

		UPROPERTY(BlueprintReadOnly, Category = "Polaris|Attributes|Combat")
	FGameplayAttributeData ReserveAmmo;
	ATTRIBUTE_ACCESSORS(UPolarisCombatAttributeSet, ReserveAmmo)

		UPROPERTY(BlueprintReadOnly, Category = "Polaris|Attributes|Combat")
	FGameplayAttributeData MaxReserveAmmo;
	ATTRIBUTE_ACCESSORS(UPolarisCombatAttributeSet, MaxReserveAmmo)

		// --- Accuracy & Damage (Spread expansion and Hitscan trace) ---
		UPROPERTY(BlueprintReadOnly, Category = "Polaris|Attributes|Combat")
	FGameplayAttributeData WeaponSpread;
	ATTRIBUTE_ACCESSORS(UPolarisCombatAttributeSet, WeaponSpread)

		UPROPERTY(BlueprintReadOnly, Category = "Polaris|Attributes|Combat")
	FGameplayAttributeData BaseDamage;
	ATTRIBUTE_ACCESSORS(UPolarisCombatAttributeSet, BaseDamage)
};