#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace PolarisGameplayTags
{
	// Combat States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Aiming);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Sprinting);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_WeaponBlocked);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Action_Firing);
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Action_Reloading);

	// Equipped States
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Equipped_Rifle);

	// Abilities
	UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Combat_Fire);
}