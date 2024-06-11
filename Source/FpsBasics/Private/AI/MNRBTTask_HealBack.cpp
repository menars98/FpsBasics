// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/MNRBTTask_HealBack.h"
#include "AIModule/Classes/AIController.h"
#include <Components/MNRAttributeComponent.h>

EBTNodeResult::Type UMNRBTTask_HealBack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	//Check Controller if there is no ai then fail 
	if (ensure(MyController))
	{
		APawn* MyPawn = Cast<APawn>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		UMNRAttributeComponent* AttributeComponent = UMNRAttributeComponent::GetAttributes(MyPawn);
		if (ensure(AttributeComponent))
		{
			AttributeComponent->ApplyHealthChange(MyPawn, AttributeComponent->GetMaxHealth());
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
