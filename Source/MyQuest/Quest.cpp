#include "Quest.h"
#include "QuestSettings.h"
#include "QuestCondition.h"
#include "QuestSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/GameModeBase.h"

AQuest::AQuest()
{
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetBoxExtent(FVector(200.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = TriggerBox;
}

void AQuest::BeginPlay()
{
	Super::BeginPlay();

	if (AGameModeBase* GameMode = GetWorld()->GetAuthGameMode())
	{
		if (UQuestSystemComponent* QuestSystem = GameMode->FindComponentByClass<UQuestSystemComponent>())
		{
			QuestSystem->RegisterQuest(this);
		}
	}

	if (!QuestSettings)
	{
		return;
	}

	for (const TSubclassOf<UQuestCondition>& ConditionClass : QuestSettings->StartConditions)
	{
		if (!ConditionClass)
		{
			continue;
		}
		UQuestCondition* Condition = NewObject<UQuestCondition>(this, ConditionClass);
		Condition->StartCondition();
		StartConditions.Add(Condition);
	}
}