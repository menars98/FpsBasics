// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponTypes.h"
#include "MNRAmmoComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAmmoPoolChanged, EWeaponType, WeaponType, int32, NewTotalAmmo, int32, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSBASICS_API UMNRAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMNRAmmoComponent();

protected:
    virtual void BeginPlay() override;

    // A map that keeps track of the total number of bullets for each bullet type.
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo", meta = (DisplayName = "Default Ammo Counts"))
    TMap<EWeaponType, int32> DefaultAmmoCounts;

	// A map that keeps track of the current number of bullets for each bullet type.
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Ammo", meta = (DisplayName = "Current Ammo Pool"))
    TMap<EWeaponType, int32> CurrentAmmoPools;

public:
    // A function to query the number of bullets from outside.
    UFUNCTION(BlueprintPure, Category = "Ammo")
    int32 GetAmmoCount(EWeaponType WeaponType) const;

    // A function to consume bullets from the bullet pool.
    UFUNCTION(BlueprintCallable, Category = "Ammo")
    bool ConsumeAmmo(EWeaponType WeaponType, int32 AmountToConsume);

    // A function to add bullets.
    UFUNCTION(BlueprintCallable, Category = "Ammo")
    void AddAmmo(EWeaponType WeaponType, int32 AmountToAdd);

    UPROPERTY(BlueprintAssignable, Category = "Ammo")
    FOnAmmoPoolChanged OnAmmoPoolChanged;
};
