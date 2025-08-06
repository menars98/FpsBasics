// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MNRAmmoComponent.h"

// Sets default values for this component's properties
UMNRAmmoComponent::UMNRAmmoComponent()
{

}

void UMNRAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// Oyun baþladýðýnda, editörde ayarladýðýmýz varsayýlan mermi sayýlarýný
	// oyun sýrasýnda kullanacaðýmýz asýl mermi havuzuna kopyalýyoruz.
	CurrentAmmoPools = DefaultAmmoCounts;
}

int32 UMNRAmmoComponent::GetAmmoCount(EWeaponType WeaponType) const
{
	// Haritada o mermi türü var mý diye kontrol et.
	// Varsa deðerini, yoksa 0 döndür.
	if (const int32* FoundAmmo = CurrentAmmoPools.Find(WeaponType))
	{
		return *FoundAmmo;
	}
	return 0;
}

bool UMNRAmmoComponent::ConsumeAmmo(EWeaponType WeaponType, int32 AmountToConsume)
{
	//Check if its negative to avoid negative subtracting 
	if (!ensure(AmountToConsume > 0))
	{
		return false;
	}

	// Find that type of bullet on the map.
	if (int32* FoundAmmo = CurrentAmmoPools.Find(WeaponType))
	{
		// Check if there is enough ammunition (your if (RifleAmmo < Delta) check).
		if (*FoundAmmo >= AmountToConsume)
		{
			*FoundAmmo -= AmountToConsume;

			// Send the signal! We are sending a negative delta so that the UI understands this as a decrease.
			OnAmmoPoolChanged.Broadcast(WeaponType, *FoundAmmo, -AmountToConsume);
			return true;
		}
	}

	// Fail if there are not enough bullets or if there are no bullets of that type.
	return false;
}

void UMNRAmmoComponent::AddAmmo(EWeaponType WeaponType, int32 AmountToAdd)
{
	//Check if its negative to avoid adding negative amount
	if (!ensure(AmountToAdd > 0.0f))
	{
		return;
	}

	// Find that bullet type on the map, or create it with a value of 0 if it doesn't exist.
	// We use ‘&’ to directly reference the value on the map.
	int32& AmmoValue = CurrentAmmoPools.FindOrAdd(WeaponType);
	AmmoValue += AmountToAdd;

	// Send the signal! The UI or other systems will hear it.
	OnAmmoPoolChanged.Broadcast(WeaponType, AmmoValue, AmountToAdd);
}




