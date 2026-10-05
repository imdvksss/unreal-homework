// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

#include "QuestSettings.h"
#include "QuestCondition.h"
#include "QuestSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"

// Sets default values
AQuest::AQuest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(200.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
}

// Called when the game starts or when spawned
void AQuest::BeginPlay()
{
	Super::BeginPlay();

	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (UQuestSystemComponent* QuestComponent = GameMode->GetComponentByClass<UQuestSystemComponent>())
		{
			QuestComponent->RegisterQuest(this);
		}
	}

	if (!ensureMsgf(QuestSettings, TEXT("QuestSettings is not set for quest %s"), *GetNameSafe(this)))
	{
		return;
	}

	for (const TSubclassOf<UQuestCondition>& ConditionTemplate : QuestSettings->StartConditions)
	{
		if (!ensureMsgf(ConditionTemplate, TEXT("Bad setup for quest %s"), *GetNameSafe(this)))
		{
			return;
		}
		UQuestCondition* QuestCondition = NewObject<UQuestCondition>(this, ConditionTemplate);
		QuestCondition->StartCondition();
		QuestCondition->OnQuestConditionCompleted.AddUObject(this, &AQuest::UpdateStartStatus);
		StartConditions.Add(QuestCondition);
	}
}

// Called every frame
void AQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AQuest::UpdateStartStatus()
{
	for (UQuestCondition* Condition : StartConditions)
	{
		if (!ensure(Condition))
		{
			return;
		}
		if (!Condition->IsCompleted())
		{
			return;
		}
	}

	SetQuestStatus(EQuestStatus::Started);

	for (const TSubclassOf<UQuestCondition>& ConditionTemplate : QuestSettings->EndConditions)
	{
		if (!ensureMsgf(ConditionTemplate, TEXT("Bad setup for quest %s"), *GetNameSafe(this)))
		{
			return;
		}
		UQuestCondition* QuestCondition = NewObject<UQuestCondition>(this, ConditionTemplate);
		QuestCondition->StartCondition();
		QuestCondition->OnQuestConditionCompleted.AddUObject(this, &AQuest::UpdateEndStatus);
		EndConditions.Add(QuestCondition);
	}
}

void AQuest::UpdateEndStatus()
{
	for (UQuestCondition* Condition : EndConditions)
	{
		if (!ensure(Condition))
		{
			return;
		}
		if (!Condition->IsCompleted())
		{
			return;
		}
	}

	SetQuestStatus(EQuestStatus::Completed);
}

void AQuest::SetQuestStatus(EQuestStatus NewStatus)
{
	if (QuestStatus == NewStatus)
	{
		return;
	}

	const EQuestStatus OldStatus = QuestStatus;
	QuestStatus = NewStatus;
	OnQuestStatusChanged.Broadcast(this, OldStatus, NewStatus);
}