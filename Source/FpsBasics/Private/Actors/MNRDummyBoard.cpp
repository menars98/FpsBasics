// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/MNRDummyBoard.h"
#include "Actors/MNRProjectile.h"
#include "Characters/MNRCharacterBase.h"
#include "Player/MNRPlayerState.h"


AMNRDummyBoard::AMNRDummyBoard()
{

	MiddleRing = CreateDefaultSubobject<UStaticMeshComponent>("MiddleRing");
	MiddleRing->SetupAttachment(OuterRing);
	MiddleRing->OnComponentHit.AddDynamic(this, &AMNRDummyBoard::OnHit);

	InnerRing = CreateDefaultSubobject<UStaticMeshComponent>("InnerRing");
	InnerRing->SetupAttachment(MiddleRing);
	InnerRing->OnComponentHit.AddDynamic(this, &AMNRDummyBoard::OnHit);

	CenterRing = CreateDefaultSubobject<UStaticMeshComponent>("CenterRing");
	CenterRing->SetupAttachment(InnerRing);
	CenterRing->OnComponentHit.AddDynamic(this, &AMNRDummyBoard::OnHit);

	OuterScore = 5.0f;
	MiddleScore = 10.0f;
	InnerScore = 30.0f;
	CenterScore = 55.0f;

}

void AMNRDummyBoard::OnRep_Down()
{
	bIsDown = !bIsDown;
	float CurrPitch = bIsDown ? TargetPitch : 0.0f;
	OuterRing->SetRelativeRotation(FRotator(0, 0, CurrPitch));
	GetWorldTimerManager().SetTimer(PitchTimerHandle, this, &AMNRDummyBoard::OnRep_Down, PitchDelay, false);
}


void AMNRDummyBoard::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
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

