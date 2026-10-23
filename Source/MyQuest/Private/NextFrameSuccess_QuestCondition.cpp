// Fill out your copyright notice in the Description page of Project Settings.


#include "NextFrameSuccess_QuestCondition.h"

void UNextFrameSuccess_QuestCondition::StartCondition()
{
	UE_LOG(LogQuest, Verbose, TEXT("Condition %s: will complete on next tick"), *GetName());
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UNextFrameSuccess_QuestCondition::Success);
}

void UNextFrameSuccess_QuestCondition::StopCondition()
{
}

void UNextFrameSuccess_QuestCondition::Success()
{
	Complete();
}