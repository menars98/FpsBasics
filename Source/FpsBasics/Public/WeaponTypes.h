#pragma once

#include "WeaponTypes.generated.h" // UENUM için bu gerekli

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    E_Rifle UMETA(DisplayName = "Rifle"),
    E_Pistol UMETA(DisplayName = "Pistol"),
    // Gelecekteki silah türleri buraya eklenecek

    E_MAX UMETA(DisplayName = "DefaultMAX") // Genellikle bir sayaç olarak kullanýlýr, zorunlu deðil.
};