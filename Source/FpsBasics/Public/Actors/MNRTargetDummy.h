// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MNRTargetDummy.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UInterpToMovementComponent;

UCLASS()
class FPSBASICS_API AMNRTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMNRTargetDummy();

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* OuterRing;

	//Link between bp & code
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathing", meta = (ExposeOnSpawn = "true", MakeEditWidget = "true"))
	TArray<FVector> PathArea;

	UPROPERTY(EditAnywhere, Category = "Credits")
	int32 OuterScore;

	UPROPERTY(EditAnywhere)
	float TargetPitch;

	UPROPERTY(EditDefaultsOnly)
	float PitchDelay;

	FTimerHandle PitchTimerHandle;

	FTimerHandle RespawnTimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_Down, VisibleDefaultsOnly, Category = Bools)
	bool bIsDown;

	UFUNCTION(BlueprintCallable)
	virtual void OnRep_Down();
	

protected:

	// BlueprintNativeEvent = C++ base implementation, can be expanded in Blueprints
	// BlueprintCallable to allow child classes to trigger OnHit
	UFUNCTION(BlueprintCallable)
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void BeginPlay() override;

private:

	//Declare moveable area
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UBoxComponent* BoxCollider;

	//Movement
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UInterpToMovementComponent* MovementComponent;
	
};
