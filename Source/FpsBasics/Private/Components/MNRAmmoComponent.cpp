// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MNRAmmoComponent.h"
#include "Player/MNRPlayerState.h"
#include "Characters/MNRCharacterBase.h"
#include <Net/UnrealNetwork.h>

UMNRAmmoComponent::UMNRAmmoComponent()
{
	StartingAmmo = 30;

	CurrentAmmo = StartingAmmo;

	MaxAmmo = 300;
}

void UMNRAmmoComponent::OnRep_Ammo(int32 OldAmmo)
{
	OnAmmoChanged.Broadcast(this->GetOwner(), this, CurrentAmmo, CurrentAmmo - OldAmmo);
}


void UMNRAmmoComponent::ReloadAmmo(AMNRCharacterBase* TargetCharacter)
{
	if (AMNRPlayerState* PS = TargetCharacter->GetPlayerState<AMNRPlayerState>())
	{
		RemoveAmmo(MissingAmmo, TargetCharacter);
	}
}

int32 UMNRAmmoComponent::GetAmmo() const
{
	return CurrentAmmo;
}

void UMNRAmmoComponent::AddAmmo(int32 Delta, AActor* InstigatorActor)
{
	//Check if its negative to avoid adding negative amount
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	CurrentAmmo += Delta;
	OnAmmoChanged.Broadcast(InstigatorActor, this, CurrentAmmo, Delta);
}

bool UMNRAmmoComponent::RemoveAmmo(int32 Delta, AActor* InstigatorActor)
{
	//Check if its negative to avoid negative subtracting 
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (CurrentAmmo < Delta)
	{
		return false;
	}

	CurrentAmmo -= Delta;
	OnAmmoChanged.Broadcast(InstigatorActor, this, CurrentAmmo, Delta);
	return true;
}

void UMNRAmmoComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMNRAmmoComponent, CurrentAmmo);
}