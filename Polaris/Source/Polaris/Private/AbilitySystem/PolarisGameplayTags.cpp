#include "AbilitySystem/PolarisGameplayTags.h"

namespace PolarisGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Aiming, "State.Aiming", "Character is currently aiming down sights (ADS)");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Sprinting, "State.Sprinting", "Character is sprinting");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_WeaponBlocked, "State.WeaponBlocked", "Weapon is obstructed by a wall or obstacle");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Action_Firing, "State.Action.Firing", "Character is firing a weapon");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Action_Reloading, "State.Action.Reloading", "Character is reloading a weapon");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Equipped_Rifle, "State.Equipped.Rifle", "Character has a rifle equipped in hands");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Combat_Fire, "Ability.Combat.Fire", "Ability to fire a weapon");
}