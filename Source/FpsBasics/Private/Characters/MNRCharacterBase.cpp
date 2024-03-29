// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MNRCharacterBase.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Actors/MNRAmmo.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AMNRCharacterBase::AMNRCharacterBase()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	bIsRifle = true;
}

void AMNRCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

void AMNRCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMNRCharacterBase::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMNRCharacterBase::Look);

		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AMNRCharacterBase::ManuelReload);

	}

}

void AMNRCharacterBase::Move(const FInputActionValue& Value)
{
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add movement 
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

void AMNRCharacterBase::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


//Below These Reload & CalculateAmmo is gonna change. They are here just for starting
//@TODO Take Reload to WeaponComponent, Calculate Ammo In here & there, substract total ammo in character, weapon clip in weaponcomp. Or we can store ammo in weapons
void AMNRCharacterBase::Reload(EWeaponType WeaponType)
{
	if (bHasRifle)
	{
		switch (WeaponType)
		{
		case EWeaponType::E_Rifle:
			if (WeaponComponent->CurrentClip != WeaponComponent->ClipSize)
			{
				if (RifleAmmo - (WeaponComponent->ClipSize - WeaponComponent->CurrentClip) >= 0)
				{
					RemoveAmmo((WeaponComponent->ClipSize - WeaponComponent->CurrentClip), WeaponType);
					WeaponComponent->CurrentClip = WeaponComponent->ClipSize;
					OnClipChanged.Broadcast(this, WeaponComponent->CurrentClip, 0);
				}
				else
				{
					WeaponComponent->CurrentClip += RifleAmmo;
					RifleAmmo = 0;
				}
			}		
			break;
		default:
			break;
		}
	}
}

int AMNRCharacterBase::CalculateAmmo(int NewAmmoAmount)
{
	//UMNRWeaponComponent* WeaponComponent = UMNRWeaponComponent::GetComponents(this);
	if (WeaponComponent->CurrentClip != WeaponComponent->ClipSize)
	{
		if (NewAmmoAmount - (WeaponComponent->ClipSize - WeaponComponent->CurrentClip) >= 0)
		{
			NewAmmoAmount -= (WeaponComponent->ClipSize - WeaponComponent->CurrentClip);
			WeaponComponent->CurrentClip = WeaponComponent->ClipSize;
			OnClipChanged.Broadcast(this, WeaponComponent->CurrentClip, 0);
		}
		else
		{
			WeaponComponent->CurrentClip += NewAmmoAmount;
			NewAmmoAmount = 0;
		}
	}
	else
	{
		//Pop Up No Ammo Message
	}

	return NewAmmoAmount;
}

void AMNRCharacterBase::ManuelReload()
{
	Reload(WeaponComponent->WeaponType);	
}

void AMNRCharacterBase::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AMNRCharacterBase::GetHasRifle()
{
	return bHasRifle;
}


void AMNRCharacterBase::OnRep_Ammo(int32 OldAmmo)
{

	if (bIsRifle)
	{
		OnAmmoChanged.Broadcast(this, RifleAmmo, RifleAmmo - OldAmmo);
	}	
}

void AMNRCharacterBase::AddAmmo(int32 Delta, EAmmoType AmmoType)
{
	//Check if its negative to avoid adding negative amount
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	switch (AmmoType)
	{
	case EAmmoType::E_Rifle:
		RifleAmmo += Delta;
		bIsRifle = true;
		OnAmmoChanged.Broadcast(this, RifleAmmo, Delta);
		break;
	default:
		break;
	}
}

bool AMNRCharacterBase::RemoveAmmo(int32 Delta, EWeaponType WeaponType)
{
	//Check if its negative to avoid negative subtracting 
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (RifleAmmo < Delta)
	{
		return false;
	}

	switch (WeaponType)
	{
	case EWeaponType::E_Rifle:
		RifleAmmo -= Delta;
		bIsRifle = true;
		OnAmmoChanged.Broadcast(this, RifleAmmo, Delta);
		break;
	default:
		break;
	}

	return true;
}


	

void AMNRCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMNRCharacterBase, AmmoAmount);
}
