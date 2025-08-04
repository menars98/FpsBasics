// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MNRBTService_CheckHealth.h"
//#include "AIModule/Classes/AIController.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include <Components/MNRAttributeComponent.h>

UMNRBTService_CheckHealth::UMNRBTService_CheckHealth()
{
		LowHealthFraction = 0.6f;
}

void UMNRBTService_CheckHealth::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();

	if (ensure(AIPawn))
	{
		UMNRAttributeComponent* AttributeComp = Cast<UMNRAttributeComponent>(AIPawn->GetComponentByClass(UMNRAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			bool bLowHealth = (AttributeComp->GetCurrentHealth() / AttributeComp->GetMaxHealth()) < LowHealthFraction;

			UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
			BlackBoardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bLowHealth);
		}
		
	}
}

	
