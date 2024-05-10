// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/MNRBlueprintFunctionLibrary.h"
#include <Components/MNRAttributeComponent.h>

bool UMNRBlueprintFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount)
{
	UMNRAttributeComponent* AttributeComp = UMNRAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp)
	{
		return AttributeComp->ApplyHealthChange(DamageCauser, -DamageAmount);
	}
	return false;
}

bool UMNRBlueprintFunctionLibrary::ApplyDirectionalDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount, const FHitResult& HitResult)
{
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount))
	{
		UPrimitiveComponent* HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName))
		{
			//Direction = Target - Origin
			FVector Direction = HitResult.TraceEnd - HitResult.TraceStart;
			Direction.Normalize();

			HitComp->AddImpulseAtLocation(-Direction * 300000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}

	return false;
}
