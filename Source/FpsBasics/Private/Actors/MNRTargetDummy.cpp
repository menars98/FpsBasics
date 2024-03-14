// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MNRTargetDummy.h"
#include "Characters/MNRCharacterBase.h"
#include "Actors/MNRProjectile.h"
#include "Player/MNRPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"
#include "Components/InterpToMovementComponent.h"

// Sets default values
AMNRTargetDummy::AMNRTargetDummy()
{
	bIsDown = false;

	BoxCollider = CreateDefaultSubobject<UBoxComponent>("BoxCollider");
	RootComponent = BoxCollider;

	OuterRing = CreateDefaultSubobject<UStaticMeshComponent>("OuterRing");
	OuterRing->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterRing->SetupAttachment(RootComponent);
	OuterRing->OnComponentHit.AddDynamic(this, &AMNRTargetDummy::OnHit);

	MovementComponent = CreateDefaultSubobject<UInterpToMovementComponent>("MovementComponent");

	//Setting default values
	//how long the entire path takes the to run
	MovementComponent->Duration = 5.0f;
	MovementComponent->bSweep = true;
	MovementComponent->BehaviourType = EInterpToBehaviourType::PingPong;

	OuterScore = 25.0f;

	TargetPitch = -80;
	PitchDelay = 3.0f;

}

void AMNRTargetDummy::OnRep_Down()
{
	if (bIsDown)
	{
		SetActorEnableCollision(!bIsDown);
		RootComponent->SetVisibility(!bIsDown, true);
		bIsDown = false;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AMNRTargetDummy::OnRep_Down, PitchDelay, false);
	}
	else
	{
		SetActorEnableCollision(!bIsDown);
		RootComponent->SetVisibility(!bIsDown, true);
	}

}

void AMNRTargetDummy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && !bIsDown)
	{
		if (AMNRProjectile* Projectile = Cast<AMNRProjectile>(OtherActor))
		{
			if (AMNRCharacterBase* Character = Cast<AMNRCharacterBase>(Projectile->GetInstigator()))
			{
				if (AMNRPlayerState* PS = Character->GetPlayerState<AMNRPlayerState>())
				{
					if (HitComp == OuterRing)
					{
						PS->AddScore(OuterScore);
					}

					bIsDown = !bIsDown;
					OnRep_Down();
				}
			}
			FString InstigatorString = FString::Printf(TEXT("Instigator is: %s"), *GetNameSafe(Projectile->GetInstigator()));
			DrawDebugString(GetWorld(), Hit.ImpactPoint, InstigatorString, nullptr, FColor::Red, 2.0f, true);
		}
		//UE_LOG(LogTemp, Warning, TEXT("OtherActor: %s, at game time %f"), *GetNameSafe(OtherActor), GetWorld()->TimeSeconds);

		FString CombinedString = FString::Printf(TEXT("Hit at Location: %s"), *GetNameSafe(HitComp));
		DrawDebugString(GetWorld(), Hit.ImpactPoint, CombinedString, nullptr, FColor::Green, 2.0f, true);
	}

}

void AMNRTargetDummy::BeginPlay()
{
	Super::BeginPlay();

	//
	MovementComponent->ControlPoints.Add(FInterpControlPoint(FVector(0.0f, 0.0f, 0.0f), true));

	for (int i = 0; i < PathArea.Num(); i++)
	{
		MovementComponent->ControlPoints.Add(FInterpControlPoint(PathArea[i], true));
	}

	MovementComponent->FinaliseControlPoints();
}

void AMNRTargetDummy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMNRTargetDummy, bIsDown);
}
