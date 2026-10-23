#include "Timer_QuestCondition.h"

void UTimer_QuestCondition::StartCondition()
{
	UE_LOG(LogQuest, Warning, TEXT("Condition %s: timer condition has no logic yet and will never complete"), *GetName());
}

void UTimer_QuestCondition::StopCondition()
{
}