// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "MNRBTService_CheckHealth.generated.h"

/**
 * 
 */
UCLASS()
class FPSBASICS_API UMNRBTService_CheckHealth : public UBTService
{
	GENERATED_BODY()

	//@TODO Check periodically if ai is low health
protected:

	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector LowHealthKey;

	UPROPERTY(EditAnywhere, Category = "AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float LowHealthFraction;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:

	UMNRBTService_CheckHealth();
	
};
