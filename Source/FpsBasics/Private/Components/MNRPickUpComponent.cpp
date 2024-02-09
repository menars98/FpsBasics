// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MNRPickUpComponent.h"

UMNRPickUpComponent::UMNRPickUpComponent()
{

}

void UMNRPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UMNRPickUpComponent::OnSphereBeginOverlap);
}

void UMNRPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AMNRCharacterBase* Character = Cast<AMNRCharacterBase>(OtherActor);
	if (Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
