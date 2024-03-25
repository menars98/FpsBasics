// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MNRAmmo.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E_Rifle UMETA(DisplayName = "Rifle")
};

UCLASS()
class FPSBASICS_API AMNRAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMNRAmmo();

	/*Create Ammo*/
	//Inside
	/*
	* int32 AmmoAmount
	* //We can declare diffrent ammo types and declare their properties in object class later.
	*/

	//How many ammo it carries
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoAmount;
	
	//Ammo type for adding
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	EAmmoType AmmoType;

	//UPROPERTY(BlueprintAssignable, Category = "Events")
	//FOnAmmoChanged OnAmmoChanged;

	/*Ammo Events*/

	//UFUNCTION()
	//void OnRep_Ammo(int32 OldAmmo);

	//UFUNCTION(BlueprintCallable, Category = "Ammo")
	//void AddAmmo(int32 Delta, AActor* InstigatorActor);

	//If we want to remove ammo from character we need a object class for it, right now we only have actor class. So this class is useless right now.
	//UFUNCTION(BlueprintCallable, Category = "Ammo")
	//bool DropAmmo(int32 Delta, AActor* InstigatorActor);
	/*
	* Ammo Events
	* 
	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 StartingAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Ammo")
	int32 MaxAmmo;

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = "OnRep_Ammo", Category = "Ammo")
	int32 CurrentAmmo;


	UFUNCTION()
	void ReloadAmmo(AMNRCharacterBase* TargetCharacter);

	UFUNCTION()
	void OnRep_Ammo(int32 OldAmmo);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAmmoChanged OnAmmoChanged;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int32 GetAmmo() const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int32 Delta, AActor* InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool RemoveAmmo(int32 Delta, AActor* InstigatorActor);*/


};
