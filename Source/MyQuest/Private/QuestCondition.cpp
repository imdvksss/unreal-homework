// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestCondition.h"

DEFINE_LOG_CATEGORY(LogQuest)

void UQuestCondition::Complete()
{
	if (bCompleted)
	{
		UE_LOG(LogQuest, Warning, TEXT("Condition %s of quest %s is already completed"), *GetName(), *GetNameSafe(GetOuter()));
		return;
	}

	UE_LOG(LogQuest, Log, TEXT("Condition %s of quest %s completed"), *GetName(), *GetNameSafe(GetOuter()));
	bCompleted = true;
	OnQuestConditionCompleted.Broadcast();
}