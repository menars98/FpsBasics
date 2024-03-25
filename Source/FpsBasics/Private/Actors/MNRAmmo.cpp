// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MNRAmmo.h"



// Sets default values
AMNRAmmo::AMNRAmmo()
{
	AmmoType = EAmmoType::E_Rifle;
	AmmoAmount = 30;
}

/*void AMNRAmmo::OnRep_Ammo(int32 OldAmmo)
{
	OnAmmoChanged.Broadcast(this->GetOwner(), this, AmmoAmount, AmmoAmount - OldAmmo);
}

void AMNRAmmo::AddAmmo(int32 Delta, AActor* InstigatorActor)
{
	//Check if its negative to avoid adding negative amount
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	AMNRCharacterBase* Character = Cast<AMNRCharacterBase>(InstigatorActor);
	Character->AmmoAmount += Delta;
	AmmoAmount += Delta;
	OnAmmoChanged.Broadcast(InstigatorActor, this, AmmoAmount, Delta);
}

//We dont use it right now but later if we want to drop ammo we can call it from here
bool AMNRAmmo::DropAmmo(int32 Delta, AActor* InstigatorActor)
{
	//Check if its negative to avoid negative subtracting 
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (AmmoAmount < Delta)
	{
		return false;
	}

	AmmoAmount -= Delta;
	OnAmmoChanged.Broadcast(InstigatorActor, this, AmmoAmount, Delta);
	return true;
}*/


/*void AMNRAmmo::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMNRAmmo, AmmoAmount);
}*/