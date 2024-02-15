// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MNRTargetDummy.h"
#include "Characters/MNRCharacterBase.h"
#include "Actors/MNRProjectile.h"
#include "Player/MNRPlayerState.h"

// Sets default values
AMNRTargetDummy::AMNRTargetDummy()
{
	bIsDown = false;
	OuterRing = CreateDefaultSubobject<UStaticMeshComponent>("OuterRing");
	OuterRing->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OuterRing->SetupAttachment(RootComponent);
	OuterRing->OnComponentHit.AddDynamic(this, &AMNRTargetDummy::OnHit);

	MiddleRing = CreateDefaultSubobject<UStaticMeshComponent>("MiddleRing");
	MiddleRing->SetupAttachment(OuterRing);
	MiddleRing->OnComponentHit.AddDynamic(this, &AMNRTargetDummy::OnHit);

	InnerRing = CreateDefaultSubobject<UStaticMeshComponent>("InnerRing");
	InnerRing->SetupAttachment(OuterRing);
	InnerRing->OnComponentHit.AddDynamic(this, &AMNRTargetDummy::OnHit);

	CenterRing = CreateDefaultSubobject<UStaticMeshComponent>("CenterRing");
	CenterRing->SetupAttachment(OuterRing);
	CenterRing->OnComponentHit.AddDynamic(this, &AMNRTargetDummy::OnHit);
	
	OuterScore = 5.0f;
	MiddleScore = 10.0f;
	InnerScore = 30.0f;
	CenterScore = 55.0f;
	
}

void AMNRTargetDummy::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
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
					if (HitComp == MiddleRing)
					{
						PS->AddScore(MiddleScore);
					}
					if (HitComp == InnerRing)
					{
						PS->AddScore(InnerScore);
					}
					if (HitComp == CenterRing)
					{
						PS->AddScore(CenterScore);
					}
					
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


