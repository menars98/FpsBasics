// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MNRProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Gameplay/MNRBlueprintFunctionLibrary.h"

// Sets default values
AMNRProjectile::AMNRProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AMNRProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Die after 3 seconds by default
	InitialLifeSpan = 1.0f;
	DamageAmount = 20.0f;
}

void AMNRProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// If the actor we collided with is ourselves or our owner, do nothing and return.
	if (OtherActor && (OtherActor == this || OtherActor == GetOwner()))
	{
		
		return;
	}

	Destroy();

	//Apply damage & impulse
	if (UMNRBlueprintFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, Hit))
	{
		
	}
}



