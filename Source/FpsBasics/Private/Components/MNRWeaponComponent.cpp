// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MNRWeaponComponent.h"
#include "Characters/MNRCharacterBase.h"
#include "Actors/MNRProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UMNRWeaponComponent::UMNRWeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	HandSocketName = "StartPoint";
}

void UMNRWeaponComponent::AttachWeapon(AMNRCharacterBase* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
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
	}
}

void UMNRWeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	// Try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
		
			APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());

			const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

			FVector HandLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
			
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			ActorSpawnParams.Instigator = Character;

			/*Logic New	*/
			//@TODO we can make subclasses for Fire logic to read more easily. /**/

			/*GetWeaponTargetingSourceLocation*/

			// Use Pawn's location as a base
			APawn* const AvatarPawn = Cast<APawn>(Character);
			check(AvatarPawn);

			const FVector SourceLoc = AvatarPawn->GetActorLocation();
			const FQuat SourceRot = AvatarPawn->GetActorQuat();

			FVector TargetingSourceLocation = SourceLoc;

			/**/

			/*GetTargetingTransform*/

			AController* SourcePawnController = AvatarPawn->GetController();

			const FVector ActorLoc = AvatarPawn->GetActorLocation();
			FQuat AimQuat = AvatarPawn->GetActorQuat();
			AController* Controller = AvatarPawn->Controller;
			//FVector SourceLoc;

			double FocalDistance = 1024.0f;
			FVector FocalLoc;

			FVector CamLoc;
			FRotator CamRot;
			bool bFoundFocus = false;

			PlayerController->GetPlayerViewPoint(CamLoc, CamRot);

			// Determine initial focal point to 
			FVector AimDir = CamRot.Vector().GetSafeNormal();
			FocalLoc = CamLoc + (AimDir * FocalDistance);

			// Move the start and focal point up in front of pawn
			if (PlayerController)
			{
				const FVector WeaponLoc = SourceLoc;
				CamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir) * AimDir);
				FocalLoc = CamLoc + (AimDir * FocalDistance);
			}
			FTransform GetTargetingTransform = FTransform(CamRot, CamLoc);

			/* PerformLocalTargeting*/

			//@TODO: Should do more complicated logic here when the player is close to a wall, etc...
			const FTransform TargetTransform = GetTargetingTransform;
			FVector AimDirection = TargetTransform.GetUnitAxis(EAxis::X);
			FVector StartTrace = TargetTransform.GetTranslation();
			FVector EndAim = StartTrace + AimDirection * 2500;

			//DrawDebugSphere(GetWorld(),StartTrace + (AimDirection * 1000.0f),3.0f,1, FColor::Red, false, 3.0f, 0, 2.0f);
			//DrawDebugLine(GetWorld(), StartTrace, StartTrace + (AimDir * 100.0f), FColor::Yellow, false, 2.0f, 0, 1.0f);

			/**/
			FHitResult Hit;
			FRotator ProjRotation;
			
			FCollisionObjectQueryParams ObjParams;
			ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
			ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
			//ObjParams.AddObjectTypesToQuery(ECC_Pawn);

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(GetOwner());

			FCollisionShape Shape;
			Shape.SetSphere(5.0f);

			bool bBlockingHit = GetWorld()->SweepSingleByObjectType(Hit, StartTrace, EndAim, FQuat::Identity, ObjParams, Shape,Params);
			FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
			FColor SphereColor = bBlockingHit ? FColor::Blue : FColor::Black;
			
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 3.0f, 1, FColor::Red, false, 3.0f, 0, 2.0f);
			// true if we got to a blocking hit (Alternative: SweepSingleByChannel with ECC_WorldDynamic)
			if (bBlockingHit)
			{
				// Adjust location to end up at crosshair look-at
				ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
				
				//DrawDebugLine(GetWorld(), TraceStart, TraceEnd , LineColor, false, 0.0f, 5.0f, 5.0f);
				//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 3.0f, 1, SphereColor, false, 2.0f, 0.0f, 5.0f);
			}
			else
			{
				// Fall-back since we failed to find any blocking hit
				ProjRotation = FRotationMatrix::MakeFromX(EndAim - HandLocation).Rotator();
				//DrawDebugLine(GetWorld(), TraceStart, TraceEnd, LineColor, false, 0.0f, 5.0f, 5.0f);
				//DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 3.0f, 1, SphereColor, false, 2.0f, 0.0f, 5.0f);
			}

			// Spawn the projectile at the muzzle
			FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
			World->SpawnActor<AMNRProjectile>(ProjectileClass, SpawnTM, ActorSpawnParams);
			UE_LOG(LogTemp, Warning, TEXT("The Instigator is:%s"), *GetNameSafe(ActorSpawnParams.Instigator));

		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
	}

	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UMNRWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}
