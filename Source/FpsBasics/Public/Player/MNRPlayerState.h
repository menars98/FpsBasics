// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MNRPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnScoreChanged, APlayerState*, Player, int32, NewScore, int32, Delta);


UCLASS()
class FPSBASICS_API AMNRPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = "OnRep_HitScore", Category = "PlayerState|Score")
	int32 HitScore;

	// OnRep_ can use a parameter containing the 'old value' of the variable it is bound to. Very useful in this case to figure out the 'delta'.
	UFUNCTION()
	void OnRep_HitScore(int32 OldScore);

	// Downside of using multicast here is that we send over more data over the net, since it's an RPC with two parameters. OnRep_ is "free" since Score is already getting replicated anyway.
	//UFUNCTION(NetMulticast, Unreliable)
	//void MulticastScore(float NewScore, float Delta);

public:

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Score")
	int32 GetHitScore() const;

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Score") // < Category|SubCategory
	void AddScore(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "PlayerState|Score")
	bool RemoveScore(int32 Delta);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnScoreChanged OnScoreChanged;

	
};
