// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/MNRAttributeComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "MNRAICharacter.generated.h"


class UAIPerceptionComponent;
class UMNRAttributeComponent;

UCLASS()
class FPSBASICS_API AMNRAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMNRAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	//Deprecated
	//UPROPERTY(VisibleAnywhere, Category = "AI|Components")
	//UPawnSensingComponent* PawnSensingComp;

	// --- PROPERTIES ---

	UPROPERTY(VisibleAnywhere, Category = "AI|Components")
	UAIPerceptionComponent* PawnSensingComp;

	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMNRAttributeComponent* AttributeComponent;

	// --- PROPERTIES END ---

	// --- FUNCTIONS ---
	UFUNCTION()
	void OnTargetPerceived(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, UMNRAttributeComponent* OwningComp, float NewHealth, float Delta);

	/// --- FUNCTIONS END ---

	//void OnPawnSeen(APawn* Pawn);
};
