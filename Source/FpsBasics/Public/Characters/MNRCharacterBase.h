// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Actors/MNRAmmo.h"
#include "Components/MNRWeaponComponent.h"
#include "MNRCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAmmoChanged, AMNRCharacterBase*, OwningActor, int32, NewAmmo, int32, Delta);

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;
class UMNRAttributeComponent;

UCLASS()
class FPSBASICS_API AMNRCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AMNRCharacterBase();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Reload Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* CrouchAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UMNRWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMNRAttributeComponent* AttributeComp;

	/*For Material Visual Effect */
	UPROPERTY(VisibleAnywhere, Category = "Effects")
	FName TimeToHitParamName;

public:	
	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool GetHasRifle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsRifle;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Ammo Types*/
	//Right now only have rifleammo later we can use pistol or others
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 RifleAmmo;

	/**/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = "OnRep_Ammo", Category = "Ammo")
	int32 AmmoAmount;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	
	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnAmmoChanged OnClipChanged;

	/*Ammo Events*/

	UFUNCTION()
	void OnRep_Ammo(int32 OldAmmo);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int32 Delta, EAmmoType AmmoType);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool RemoveAmmo(int32 Delta, EWeaponType WeaponType);


protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void StartCrouch();
	
	void StopCrouch();

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMNRAttributeComponent* OwningComp, float NewHealth, float Delta);

public:

	void ManuelReload();
	
	void Reload(EWeaponType WeaponType);

	int CalculateAmmo(int NewAmmoAmount);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(Exec)
	void HealSelf(float Amount = 100);

	UFUNCTION(Exec)
	void DamageSelf(float Amount = 50);

protected:

	virtual void PostInitializeComponents() override;

};
