// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSettings.h"

#include "QuestCondition.h"

void UQuestSettings::PostLoad()
{
	Super::PostLoad();

	if (!StartConditions_DEPRECATED.IsEmpty())
	{
		UE_LOG(LogQuest, Log, TEXT("QuestSettings %s: migrating %d deprecated start conditions"),
			*GetName(), StartConditions_DEPRECATED.Num());

		for (const TSubclassOf<UQuestCondition>& ConditionTemplate : StartConditions_DEPRECATED)
		{
			if (!ensureMsgf(ConditionTemplate, TEXT("Bad setup for quest %s"), *GetNameSafe(this)))
			{
				return;
			}
			UQuestCondition* QuestCondition = NewObject<UQuestCondition>(this, ConditionTemplate);
			StartConditionsInst.Add(QuestCondition);
		}
		StartConditions_DEPRECATED.Empty();
	}

	if (!EndConditions_DEPRECATED.IsEmpty())
	{
		UE_LOG(LogQuest, Log, TEXT("QuestSettings %s: migrating %d deprecated end conditions"),
			*GetName(), EndConditions_DEPRECATED.Num());

		for (const TSubclassOf<UQuestCondition>& ConditionTemplate : EndConditions_DEPRECATED)
		{
			if (!ensureMsgf(ConditionTemplate, TEXT("Bad setup for quest %s"), *GetNameSafe(this)))
			{
				return;
			}
			UQuestCondition* QuestCondition = NewObject<UQuestCondition>(this, ConditionTemplate);
			EndConditionsInst.Add(QuestCondition);
		}
		EndConditions_DEPRECATED.Empty();
	}
}