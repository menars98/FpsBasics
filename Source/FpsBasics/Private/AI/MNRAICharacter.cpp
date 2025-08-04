// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MNRAICharacter.h"
#include "Perception/AIPerceptionComponent.h" 
//#include "Perception/PawnSensingComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

AMNRAICharacter::AMNRAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UAIPerceptionComponent>("PawnSensingComp");
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	AttributeComponent = CreateDefaultSubobject<UMNRAttributeComponent>(TEXT("AttributeComponent"));


	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SetMaxAge(5.0f);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	PawnSensingComp->ConfigureSense(*SightConfig);
	PawnSensingComp->SetDominantSense(SightConfig->GetSenseImplementation());

	PawnSensingComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMNRAICharacter::OnTargetPerceived);
	

}

void AMNRAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AttributeComponent->OnHealthChanged.AddDynamic(this, &AMNRAICharacter::OnHealthChanged);
}

void AMNRAICharacter::OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Spotted: %s"), *Actor->GetName());

		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
			if (BBComp)
			{
				BBComp->SetValueAsObject("TargetActor", Actor);

				DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
			}
		}
	}
}

void AMNRAICharacter::OnHealthChanged(AActor* InstigatorActor, UMNRAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (NewHealth <= 0.0f && Delta <= 0.0f)
	{
		// Handle death logic here
		UE_LOG(LogTemp, Warning, TEXT("AI Character %s has died."), *GetName());
		
		AAIController* AIC = Cast<AAIController>(GetController());
		if (AIC)
		{
			UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();
			if (BBComp)
			{
				BBComp->ClearValue("TargetActor");

			}
			AIC->UnPossess();
		}
		SetActorEnableCollision(false);
		// Optionally destroy the AI character
		SetLifeSpan(5.0f);
	}
}
//Deprecated
/*void AMNRAICharacter::OnPawnSeen(APawn* Pawn)
{
	AAIController* AIC = Cast<AAIController>(GetController());
	if (AIC)
	{
		UBlackboardComponent* BBComp = AIC->GetBlackboardComponent();

		BBComp->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "Player Spotted", nullptr, FColor::White, 4.0f, true);
	}
}*/


