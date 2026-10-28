#pragma once

#include "CoreMinimal.h"
#include "PolarisTypes.generated.h"

UENUM(BlueprintType)
enum class EPolarisOverlayState : uint8
{
	Unarmed		UMETA(DisplayName = "Unarmed"),
	Rifle		UMETA(DisplayName = "Rifle"),
	Pistol		UMETA(DisplayName = "Pistol")
};