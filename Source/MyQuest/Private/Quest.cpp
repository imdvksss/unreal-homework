// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

#include "QuestSettings.h"
#include "QuestCondition.h"
#include "QuestSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"

int32 bDebugInstantFinish = 0;
FAutoConsoleVariableRef CVarDebugInstantFinish(TEXT("quest.InstantFinish"),
	bDebugInstantFinish, TEXT("Debug Instantly Finish Quests"),
	ECVF_Cheat);

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

	UQuestSystemComponent* QuestComponent = nullptr;
	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		QuestComponent = GameMode->GetComponentByClass<UQuestSystemComponent>();
	}

	if (QuestComponent)
	{
		QuestComponent->RegisterQuest(this);
	}
	else
	{
		UE_LOG(LogQuest, Warning, TEXT("Quest %s: QuestSystemComponent not found in GameMode, quest is not registered"), *GetName());
	}

	if (!QuestSettings)
	{
		UE_LOG(LogQuest, Error, TEXT("Quest %s: QuestSettings is not set, quest will not start"), *GetName());
		return;
	}

	UE_LOG(LogQuest, Log, TEXT("Quest %s (%s): BeginPlay, %d start conditions"),
		*GetName(), *QuestSettings->Name.ToString(), QuestSettings->StartConditionsInst.Num());

	for (const UQuestCondition* ConditionTemplate : QuestSettings->StartConditionsInst)
	{
		if (!ensureMsgf(ConditionTemplate, TEXT("Bad setup for quest %s"), *GetNameSafe(this)))
		{
			return;
		}
		UQuestCondition* QuestCondition = DuplicateObject<UQuestCondition>(ConditionTemplate, this);
		UE_LOG(LogQuest, Verbose, TEXT("Quest %s: starting start condition %s"), *GetName(), *QuestCondition->GetName());
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
	int32 CompletedCount = 0;
	for (UQuestCondition* Condition : StartConditions)
	{
		if (!ensure(Condition))
		{
			return;
		}
		if (Condition->IsCompleted())
		{
			++CompletedCount;
		}
	}

	UE_LOG(LogQuest, Verbose, TEXT("Quest %s: start conditions %d/%d"), *GetName(), CompletedCount, StartConditions.Num());
	if (CompletedCount < StartConditions.Num())
	{
		return;
	}

	SetQuestStatus(EQuestStatus::Started);

	if (bDebugInstantFinish)
	{
		UE_LOG(LogQuest, Warning, TEXT("Quest %s: finished instantly by quest.InstantFinish"), *GetName());
		SetQuestStatus(EQuestStatus::Completed);
		return;
	}

	for (const UQuestCondition* ConditionTemplate : QuestSettings->EndConditionsInst)
	{
		if (!ensureMsgf(ConditionTemplate, TEXT("Bad setup for quest %s"), *GetNameSafe(this)))
		{
			return;
		}
		UQuestCondition* QuestCondition = DuplicateObject<UQuestCondition>(ConditionTemplate, this);
		UE_LOG(LogQuest, Verbose, TEXT("Quest %s: starting end condition %s"), *GetName(), *QuestCondition->GetName());
		QuestCondition->StartCondition();
		QuestCondition->OnQuestConditionCompleted.AddUObject(this, &AQuest::UpdateEndStatus);
		EndConditions.Add(QuestCondition);
	}
}

void AQuest::UpdateEndStatus()
{
	int32 CompletedCount = 0;
	for (UQuestCondition* Condition : EndConditions)
	{
		if (!ensure(Condition))
		{
			return;
		}
		if (Condition->IsCompleted())
		{
			++CompletedCount;
		}
	}

	UE_LOG(LogQuest, Verbose, TEXT("Quest %s: end conditions %d/%d"), *GetName(), CompletedCount, EndConditions.Num());
	if (CompletedCount < EndConditions.Num())
	{
		return;
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

	UE_LOG(LogQuest, Log, TEXT("Quest %s: %s -> %s"), *GetName(),
		*UEnum::GetValueAsString(OldStatus), *UEnum::GetValueAsString(NewStatus));
	OnQuestStatusChanged.Broadcast(this, OldStatus, NewStatus);
}