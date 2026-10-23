// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestCondition.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogQuest, Log, All);
DECLARE_MULTICAST_DELEGATE(FOnQuestConditionCompleted)

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew)
class MYQUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual void StartCondition() PURE_VIRTUAL(UQuestCondition::StartCondition,);
	virtual void StopCondition() PURE_VIRTUAL(UQuestCondition::StopCondition,);

	UFUNCTION(BlueprintCallable)
	bool IsCompleted() const { return bCompleted; }

	FOnQuestConditionCompleted OnQuestConditionCompleted;

protected:
	void Complete();

private:
	bool bCompleted = false;
};