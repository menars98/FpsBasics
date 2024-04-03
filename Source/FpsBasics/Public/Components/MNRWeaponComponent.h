// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "MNRWeaponComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnClipChanged, AActor*, InstigatorActor, int32, NewAmmo, int32, Delta);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged, UMNRWeaponComponent*, WeaponComponent, AMNRCharacterBase*, OwningActor);

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Rifle UMETA(DisplayName = "Rifle")
};


class AMNRCharacterBase;
class UMNRAmmoComponent;
class AMNRAmmo;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	EWeaponType WeaponType;

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

public:

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FName HandSocketName;

	/* Clip for Weapon */
	//How many ammo that weapon can have, Rifle-Clip 30, Deagle-Clip 6 etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Ammo")
	int32 ClipSize; 

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = "OnRep_CurrentClip", BlueprintReadOnly, Category = "Ammo")
	int32 CurrentClip;

	UPROPERTY()
	int32 MissingAmmo;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	AMNRAmmo* AmmoActor;

	// I dont use this but i left it maybe i can use
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnClipChanged OnClipChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnWeaponChanged OnWeaponChanged;

public:

	UFUNCTION()
	void ReloadClip(AMNRCharacterBase* TargetCharacter);

	UFUNCTION()
	void OnRep_CurrentClip(int32 OldAmmo);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool DecreaseAmmo(int32 Delta, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Components")
	static UMNRWeaponComponent* GetComponents(AActor* FromActor);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetWeapon(UMNRWeaponComponent* WeaponComp, AMNRCharacterBase* OwningCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetMaxAmmo();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	int32 GetCurrentAmmo();

	/*Reload Function We can declare it here or character. I guess character is better choice*/
	//UFUNCTION()
	//void Reload() {if(WeaponComponent){if(AmmoComponent)} {then (Clip - Remaining clip ammo)  } }

private:
	/** The Character holding this weapon*/
	AMNRCharacterBase* Character;
	

};
