// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MNRAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include <AIController.h>
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

AMNRAICharacter::AMNRAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
}

void AMNRAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AMNRAICharacter::OnPawnSeen);
}

void AMNRAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
	}
}


