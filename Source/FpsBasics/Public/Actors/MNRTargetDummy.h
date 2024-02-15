// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MNRTargetDummy.generated.h"

UCLASS()
class FPSBASICS_API AMNRTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMNRTargetDummy();

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* OuterRing;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* MiddleRing;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* InnerRing;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* CenterRing;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 OuterScore;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 MiddleScore;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 InnerScore;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 CenterScore;

	UPROPERTY(VisibleDefaultsOnly, Category = Bools)
	bool bIsDown;

public:

	UFUNCTION(BlueprintCallable)
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
