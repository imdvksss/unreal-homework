#include "TriggerVolume_QuestCondition.h"
#include "Quest.h"

void UTriggerVolume_QuestCondition::StartCondition()
{
	UE_LOG(LogTemp, Warning, TEXT("[TriggerCond] StartCondition called"));

	AQuest* Quest = GetTypedOuter<AQuest>();
	if (!Quest)
	{
		UE_LOG(LogTemp, Error, TEXT("[TriggerCond] Quest not found!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[TriggerCond] Subscribing to %s"),
		TriggerEvent == ETriggerVolumeEvent::OnEnter ? TEXT("OnEnter") : TEXT("OnExit"));

	if (TriggerEvent == ETriggerVolumeEvent::OnEnter)
	{
		Quest->OnActorBeginOverlap.AddDynamic(this, &UTriggerVolume_QuestCondition::HandleOverlap);
	}
	else
	{
		Quest->OnActorEndOverlap.AddDynamic(this, &UTriggerVolume_QuestCondition::HandleOverlap);
	}
}

void UTriggerVolume_QuestCondition::StopCondition()
{
	UE_LOG(LogTemp, Warning, TEXT("[TriggerCond] StopCondition called"));

	if (AQuest* Quest = GetTypedOuter<AQuest>())
	{
		Quest->OnActorBeginOverlap.RemoveDynamic(this, &UTriggerVolume_QuestCondition::HandleOverlap);
		Quest->OnActorEndOverlap.RemoveDynamic(this, &UTriggerVolume_QuestCondition::HandleOverlap);
	}
}

void UTriggerVolume_QuestCondition::HandleOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("[TriggerCond] Overlap! Other=%s"),
		OtherActor ? *OtherActor->GetName() : TEXT("null"));

	if (bCompleted || !OtherActor)
	{
		return;
	}

	if (OtherTag.IsNone() || OtherActor->ActorHasTag(OtherTag))
	{
		bCompleted = true;
		UE_LOG(LogTemp, Warning, TEXT("[TriggerCond] CONDITION COMPLETED"));
		StopCondition();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[TriggerCond] Tag mismatch, skipping"));
	}
}