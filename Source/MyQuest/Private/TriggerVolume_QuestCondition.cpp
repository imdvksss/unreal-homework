// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerVolume_QuestCondition.h"

#include "Quest.h"
#include "VisualLogger/VisualLogger.h"

void UTriggerVolume_QuestCondition::StartCondition()
{
	AQuest* Quest = Cast<AQuest>(GetOuter());
	if (!Quest)
	{
		UE_LOG(LogQuest, Error, TEXT("Condition %s: outer is not a Quest, condition will never complete"), *GetName());
		return;
	}

	UE_LOG(LogQuest, Log, TEXT("Condition %s: waiting for %s of actor with tag '%s' in quest %s"), *GetName(),
		TriggerEvent == ETriggerVolumeEvent::OnEnter ? TEXT("enter") : TEXT("exit"),
		*OtherTag.ToString(), *Quest->GetName());

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

	const bool bTagMatches = OtherTag.IsNone() || OtherActor->ActorHasTag(OtherTag);
	UE_LOG(LogQuest, Verbose, TEXT("Condition %s: %s actor %s (%s)"), *GetName(),
		TriggerEvent == ETriggerVolumeEvent::OnEnter ? TEXT("entered") : TEXT("exited"),
		*OtherActor->GetName(), bTagMatches ? TEXT("tag matches") : TEXT("no tag"));

	if (AQuest* Quest = Cast<AQuest>(GetOuter()))
	{
		FVector Center;
		FVector Extents;
		Quest->GetActorBounds(true, Center, Extents, false);
		UE_VLOG_BOX(Quest, LogQuest, Log, FBox(Center - Extents, Center + Extents), FColor::Orange, TEXT("%s"), *GetName());
	}

	if (bTagMatches)
	{
		StopCondition();
		Complete();
	}
}