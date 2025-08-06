// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MNRWeaponComponent.h"
#include "Characters/MNRCharacterBase.h"
#include "Actors/MNRProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MNRAmmoComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/MNRAmmo.h"
#include <Net/UnrealNetwork.h>

UMNRWeaponComponent::UMNRWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	HandSocketName = "StartPoint";

	WeaponType = EWeaponType::E_Rifle;
	ClipSize = 30;
	CurrentClip = 30;
}

void UMNRWeaponComponent::AttachWeapon(AMNRCharacterBase* TargetCharacter)
{
	CharacterOwner = TargetCharacter;
	if (CharacterOwner == nullptr)
	{
		return;
	}
	/*
	// Attach the weapon to the First Person CharacterOwner
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(CharacterOwner->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));
	//How we setup rules
	//SetupAttachment(CharacterOwner->GetMesh1P(), FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	CharacterOwner->SetHasRifle(true);
	SetWeapon(this, CharacterOwner);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(CharacterOwner->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UMNRWeaponComponent::Fire);
	
		}
	}*/
}

void UMNRWeaponComponent::Fire()
{

	if (CharacterOwner == nullptr || CharacterOwner->GetController() == nullptr || ProjectileClass == nullptr)
	{
		return;
	}

	if (CurrentClip <= 0)
	{
		Reload();
		return;
	}

	FTransform AimTransform;
	if (!GetAimingTransform(AimTransform))
	{
		// If the aiming direction cannot be obtained, do not fire.
		return;
	}

	FHitResult AimHitResult;
	const bool bHitTarget = FindAimTarget(AimTransform, AimHitResult);

	const FVector MuzzleLocation = GetOwner()->GetActorLocation() + AimTransform.GetRotation().RotateVector(MuzzleOffset);
	const FRotator ProjectileRotation = CalculateProjectileRotation(MuzzleLocation, AimHitResult);

	// Spawn Bullet
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		SpawnParams.Instigator = CharacterOwner;
		SpawnParams.Owner = CharacterOwner;

		FTransform SpawnTM = FTransform(ProjectileRotation, MuzzleLocation);
		World->SpawnActor<AMNRProjectile>(ProjectileClass, SpawnTM, SpawnParams);
	}

	DecreaseAmmo(1, CharacterOwner);

	// Play feedback
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, CharacterOwner->GetActorLocation());
	}

	if (FireAnimation)
	{
		if (UAnimInstance* AnimInstance = CharacterOwner->GetMesh1P()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UMNRWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (CharacterOwner == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(CharacterOwner->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

void UMNRWeaponComponent::ReloadClip(AMNRCharacterBase* TargetCharacter)
{
	if (CurrentClip < ClipSize)
	{
		
		//@TODO Broadcast for clip

	}
}

void UMNRWeaponComponent::OnRep_CurrentClip(int32 OldAmmo)
{
	OnClipChanged.Broadcast(this->GetOwner(), CurrentClip, CurrentClip - OldAmmo);
}

bool UMNRWeaponComponent::DecreaseAmmo(int32 Delta, AActor* InstigatorActor)
{
	//Check if its negative to avoid negative subtracting 
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (CurrentClip < Delta)
	{
		return false;
	}

	CurrentClip -= Delta;
	OnClipChanged.Broadcast(InstigatorActor, CurrentClip, Delta);
	return true;
}

UMNRWeaponComponent* UMNRWeaponComponent::GetComponents(AActor* FromActor)
{
	if (FromActor)
	{
		return FromActor->FindComponentByClass<UMNRWeaponComponent>();
	}
	return nullptr;
}

void UMNRWeaponComponent::SetWeapon(UMNRWeaponComponent* WeaponComp, AActor* OwningCharacter)
{
	OnWeaponChanged.Broadcast(WeaponComp, OwningCharacter);
}

void UMNRWeaponComponent::SetCharacterOwner(AMNRCharacterBase* NewOwner)
{
	CharacterOwner = NewOwner;
}

int32 UMNRWeaponComponent::GetMaxAmmo()
{
	return ClipSize;
}

int32 UMNRWeaponComponent::GetCurrentAmmo()
{
	return CurrentClip;
}

void UMNRWeaponComponent::Reload()
{
	// If it is already full or the magazine loading animation is playing, do nothing.
	if (CurrentClip == ClipSize /*|| bIsReloading*/)
	{
		return;
	}

	// Find the character we own and its AmmoComponent.
	if (!CharacterOwner) return;

	UMNRAmmoComponent* AmmoComp = CharacterOwner->FindComponentByClass<UMNRAmmoComponent>();
	if (!AmmoComp) return;

	// How many bullets do we need to fill the magazine?
	const int32 AmmoNeeded = ClipSize - CurrentClip;
	if (AmmoNeeded <= 0) return;

	// Try to get the bullet we need from AmmoComponent.
	const int32 AmmoToTake = AmmoComp->GetAmmoCount(WeaponType);
	const int32 AmmoToMove = FMath::Min(AmmoNeeded, AmmoToTake);

	if (AmmoToMove > 0)
	{
		// If we have ammunition, consume it from AmmoComponent.
		if (AmmoComp->ConsumeAmmo(WeaponType, AmmoToMove))
		{
			// @TODO: Play the animation and sounds here. Start a timer.
			// When the timer expires, execute the following line.
			CurrentClip += AmmoToMove;
			OnClipChanged.Broadcast(this->GetOwner(), CurrentClip, 0); // Update UI
		}
	}
}

bool UMNRWeaponComponent::GetAimingTransform(FTransform& OutAimTransform) const
{
	APlayerController* PlayerController = Cast<APlayerController>(CharacterOwner->GetController());
	if (!PlayerController)
	{
		// @TODO: AI targeting logic can be added here.
		// For now, we only support the player.
		return false;
	}

	FVector CamLoc;
	FRotator CamRot;
	PlayerController->GetPlayerViewPoint(CamLoc, CamRot);
	OutAimTransform = FTransform(CamRot, CamLoc);
	return true;
}

bool UMNRWeaponComponent::FindAimTarget(const FTransform& AimTransform, FHitResult& OutHitResult) const
{
	const FVector StartTrace = AimTransform.GetTranslation();
	const FVector AimDirection = AimTransform.GetUnitAxis(EAxis::X);
	const FVector EndTrace = StartTrace + AimDirection * 2500.0f; // Range

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjParams.AddObjectTypesToQuery(ECC_Pawn); // To shoot characters too

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.AddIgnoredActor(CharacterOwner);

	return GetWorld()->LineTraceSingleByObjectType(OutHitResult, StartTrace, EndTrace, ObjParams, Params);
}

FRotator UMNRWeaponComponent::CalculateProjectileRotation(const FVector& MuzzleLocation, const FHitResult& AimHitResult) const
{
	FVector AimDirection;
	if (AimHitResult.bBlockingHit)
	{
		// If we hit a target, aim from the barrel toward that point.
		AimDirection = AimHitResult.ImpactPoint - MuzzleLocation;
	}
	else
	{
		// If we didn't hit anything, aim at the furthest point from the barrel.
		AimDirection = AimHitResult.TraceEnd - MuzzleLocation;
	}

	return AimDirection.Rotation();

}

void UMNRWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UMNRWeaponComponent, CurrentClip);
}