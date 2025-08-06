#pragma once

#include "WeaponTypes.generated.h" // UENUM i�in bu gerekli

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    E_Rifle UMETA(DisplayName = "Rifle"),
    E_Pistol UMETA(DisplayName = "Pistol"),
    // Gelecekteki silah t�rleri buraya eklenecek

    E_MAX UMETA(DisplayName = "DefaultMAX") // Genellikle bir saya� olarak kullan�l�r, zorunlu de�il.
};