// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MNRPlayerState.h"
#include "Net/UnrealNetwork.h"


void AMNRPlayerState::OnRep_HitScore(int32 OldScore)
{
	OnScoreChanged.Broadcast(this, HitScore, HitScore - OldScore);
}

int32 AMNRPlayerState::GetHitScore() const
{
	return HitScore;
}

void AMNRPlayerState::AddScore(int32 Delta)
{
	//Check if its negative to avoid adding negative amount
	if (!ensure(Delta > 0.0f))
	{
		return;
	}
	HitScore += Delta;
	OnScoreChanged.Broadcast(this, HitScore, Delta);
}

bool AMNRPlayerState::RemoveScore(int32 Delta)
{
	//Check if its negative to avoid negative subtracting 
	if (!ensure(Delta > 0.0f))
	{
		return false;
	}

	if (HitScore < Delta)
	{
		return false;
	}

	HitScore -= Delta;
	OnScoreChanged.Broadcast(this, HitScore, Delta);
	return true;
}

//In case of if we are changing multiplayer
void AMNRPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMNRPlayerState, HitScore);
}