// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MNRBTTask_HealBack.generated.h"

/**
 * 
 */
UCLASS()
class FPSBASICS_API UMNRBTTask_HealBack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//@TODO If its low health, heal back to maxhitpoint

protected:



	
};
