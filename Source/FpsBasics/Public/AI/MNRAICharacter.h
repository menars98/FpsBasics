// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MNRAICharacter.generated.h"

class UPawnSensingComponent;

UCLASS()
class FPSBASICS_API AMNRAICharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AMNRAICharacter();

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "AI|Components")
	UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	void OnPawnSeen(APawn* Pawn);
};
