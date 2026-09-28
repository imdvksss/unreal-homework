#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestCondition.generated.h"

UCLASS(Abstract, Blueprintable)
class MYQUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual void StartCondition() PURE_VIRTUAL(UQuestCondition::StartCondition,);
	virtual void StopCondition() PURE_VIRTUAL(UQuestCondition::StopCondition,);

	UFUNCTION(BlueprintPure)
	bool IsCompleted() const { return bCompleted; }

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bCompleted = false;
};