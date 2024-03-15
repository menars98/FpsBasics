// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MNRWeaponComponent.generated.h"


class AMNRCharacterBase;
class UMNRAmmoComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FPSBASICS_API UMNRWeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AMNRProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Sets default values for this component's properties */
	UMNRWeaponComponent();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AttachWeapon(AMNRCharacterBase* TargetCharacter);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, Category ="Weapon")
	FName HandSocketName;

	/* Clip for Weapon */
	//How many ammo that weapon can have, Rifle-Clip 30, Deagle-Clip 6 etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Ammo")
	int32 ClipSize; 

	UPROPERTY()
	int32 CurrentClip;

	UPROPERTY()
	int32 MissingAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMNRAmmoComponent* AmmoComponent;

public:

	UFUNCTION()
	void ReloadClip(AMNRCharacterBase* TargetCharacter);

	/*Reload Function We can declare it here or character. I guess character is better choice*/
	//UFUNCTION()
	//void Reload() {if(WeaponComponent){if(AmmoComponent)} {then (Clip - Remaining clip ammo)  } }

private:
	/** The Character holding this weapon*/
	AMNRCharacterBase* Character;
	

};
