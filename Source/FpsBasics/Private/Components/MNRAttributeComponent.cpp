// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MNRAttributeComponent.h"
#include <Net/UnrealNetwork.h>

static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global Damage Modifier"), ECVF_Cheat);

UMNRAttributeComponent::UMNRAttributeComponent()
{
	MaxHealth = 100.0f;
	Health = MaxHealth;

	SetIsReplicatedByDefault(true);
}

UMNRAttributeComponent* UMNRAttributeComponent::GetAttributes(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UMNRAttributeComponent>();
		//return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}
	return nullptr;
}

bool UMNRAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetMaxHealth());
}

float UMNRAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

float UMNRAttributeComponent::GetCurrentHealth() const
{
	return Health;
}

bool UMNRAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged())
	{
		return false;
	}

	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread(); //@fixme: You can specify damage to different actors or pawn, like ai,explosion,player etc or receive dmg.

		Delta *= DamageMultiplier;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(FMath::RoundHalfFromZero(Health + Delta), 0.0f, MaxHealth);
	float ActualDelta = NewHealth - OldHealth;

	//Is Server?
	if (GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if (ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		//Died
		if (ActualDelta < 0.0f && Health == 0.0f)
		{
			//AMNRGameModeBase* GM = GetWorld()->GetAuthGameMode<AMNRGameModeBase>();
			//if (GM)
			//{
			//	GM->OnActorKilled(GetOwner(), InstigatorActor);
			//}
		}
	}

	return ActualDelta != 0;
}

bool UMNRAttributeComponent::IsActorAlive(AActor* Actor)
{
	UMNRAttributeComponent* AttributeComp = UMNRAttributeComponent::GetAttributes(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

bool UMNRAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool UMNRAttributeComponent::IsMaxHealth() const
{
	return Health == MaxHealth;
}

bool UMNRAttributeComponent::IsLowHealth() const
{
	return Health <= 50;
}

void UMNRAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewValue, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewValue, Delta);
}

void UMNRAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMNRAttributeComponent, Health);
	DOREPLIFETIME_CONDITION(UMNRAttributeComponent, MaxHealth, COND_OwnerOnly); //For optimization

}

