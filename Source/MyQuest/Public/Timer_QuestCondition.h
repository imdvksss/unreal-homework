#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "Timer_QuestCondition.generated.h"

UCLASS()
class MYQUEST_API UTimer_QuestCondition : public UQuestCondition
{
	GENERATED_BODY()

public:
	virtual void StartCondition() override;
	virtual void StopCondition() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Condition")
	float Duration = 5.f;
};