// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystemComponent.h"

#include "EngineUtils.h"
#include "Quest.h"
#include "QuestCondition.h"

// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	for (const TSubclassOf<AQuest>& QuestClass : Quests)
	{
		AQuest* Quest = GetWorld()->SpawnActor<AQuest>(QuestClass);
		if (!Quest)
		{
			UE_LOG(LogQuest, Error, TEXT("QuestSystem: failed to spawn quest of class %s"), *GetNameSafe(QuestClass.Get()));
			continue;
		}
		UE_LOG(LogQuest, Log, TEXT("QuestSystem: spawned quest %s"), *Quest->GetName());
		RegisterQuest(Quest);
	}
}


// Called every frame
void UQuestSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestSystemComponent::GetActiveAndStartedQuests(TArray<AQuest*>& OutQuests)
{
	for (AQuest* Quest : ActiveQuests)
	{
		if (Quest && Quest->GetQuestStatus() == EQuestStatus::Started)
		{
			OutQuests.Add(Quest);
		}
	}
}

void UQuestSystemComponent::RegisterQuest(AQuest* NewQuest)
{
	if (!NewQuest)
	{
		UE_LOG(LogQuest, Warning, TEXT("QuestSystem: tried to register null quest"));
		return;
	}

	if (ActiveQuests.Contains(NewQuest))
	{
		UE_LOG(LogQuest, Verbose, TEXT("QuestSystem: quest %s is already registered"), *NewQuest->GetName());
		return;
	}

	ActiveQuests.Add(NewQuest);
	UE_LOG(LogQuest, Log, TEXT("QuestSystem: registered quest %s (total %d)"), *NewQuest->GetName(), ActiveQuests.Num());
	NewQuest->OnQuestStatusChanged.AddUniqueDynamic(this, &UQuestSystemComponent::HandleQuestStatusChanged);
}

void UQuestSystemComponent::HandleQuestStatusChanged(AQuest* Quest, EQuestStatus OldStatus, EQuestStatus NewStatus)
{
	UE_LOG(LogQuest, Verbose, TEXT("QuestSystem: received status change of %s: %s -> %s"),
		*GetNameSafe(Quest),
		*UEnum::GetValueAsString(OldStatus),
		*UEnum::GetValueAsString(NewStatus));
}