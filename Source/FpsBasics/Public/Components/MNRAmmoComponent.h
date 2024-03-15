// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MNRWeaponComponent.h"
#include "MNRAmmoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnAmmoChanged, AActor*, InstigatorActor, UMNRAmmoComponent*, OwningComp, int32, NewAmmo, int32, Delta);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSBASICS_API UMNRAmmoComponent : public UMNRWeaponComponent
{
	GENERATED_BODY()
	

	UMNRAmmoComponent();

	/*@TODO Change AmmoComponent to independent component*/
	/*Create Ammo Component*/
	//Inside
	/*
	* int32 Current Ammo
	* int32 Starting Ammo
	* //Maybe Diffrent Ammo types we can create struct and declare properities inside like thrust speed etc.
	* //struct AmmoType{float thrust, float speed, bool bCanExplode}
	* int32 Max Ammo
	*
	*/

public:

	UPROPERTY()
	int32 StartingAmmo;

	UPROPERTY()
	int32 MaxAmmo;

	UFUNCTION()
	void ReloadAmmo(AMNRCharacterBase* TargetCharacter);
	
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = "OnRep_Ammo", Category = "Ammo")
	int32 CurrentAmmo;

	UFUNCTION()
	void OnRep_Ammo(int32 OldAmmo);

	/*Ammo Events*/
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int32 Delta, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool RemoveAmmo(int32 Delta, AActor* InstigatorActor);
};
