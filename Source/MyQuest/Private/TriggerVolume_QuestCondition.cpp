// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_QuestCondition.h"

#include "Quest.h"

void UTriggerVolume_QuestCondition::StartCondition()
{
	AQuest* Quest = Cast<AQuest>(GetOuter());
	if (!ensure(Quest))
	{
		return;
	}

	if (TriggerEvent == ETriggerVolumeEvent::OnEnter)
	{
		Quest->OnActorBeginOverlap.AddDynamic(this, &UTriggerVolume_QuestCondition::StartOverlap);
	}
	else
	{
		Quest->OnActorEndOverlap.AddDynamic(this, &UTriggerVolume_QuestCondition::StartOverlap);
	}
}

void UTriggerVolume_QuestCondition::StopCondition()
{
	if (AQuest* Quest = Cast<AQuest>(GetOuter()))
	{
		Quest->OnActorBeginOverlap.RemoveDynamic(this, &UTriggerVolume_QuestCondition::StartOverlap);
		Quest->OnActorEndOverlap.RemoveDynamic(this, &UTriggerVolume_QuestCondition::StartOverlap);
	}
}

void UTriggerVolume_QuestCondition::StartOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (IsCompleted() || !OtherActor)
	{
		return;
	}

	if (OtherTag.IsNone() || OtherActor->ActorHasTag(OtherTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("Condition %s completed (%s)"),
			*GetName(),
			TriggerEvent == ETriggerVolumeEvent::OnEnter ? TEXT("enter") : TEXT("exit"));
		StopCondition();
		Complete();
	}
}