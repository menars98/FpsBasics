// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/MNRTargetDummy.h"
#include "MNRDummyBoard.generated.h"

/**
 * 
 */
UCLASS()
class FPSBASICS_API AMNRDummyBoard : public AMNRTargetDummy
{
	GENERATED_BODY()
	
public:

	AMNRDummyBoard();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* MiddleRing;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* InnerRing;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* CenterRing;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 MiddleScore;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 InnerScore;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CenterScore;

protected:

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	void OnRep_Down() override;

};
