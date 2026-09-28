#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

class UBoxComponent;
class UQuestCondition;
class UQuestSettings;

UCLASS()
class MYQUEST_API AQuest : public AActor
{
	GENERATED_BODY()

public:
	AQuest();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UQuestSettings> QuestSettings;

public:
	UPROPERTY()
	TArray<TObjectPtr<UQuestCondition>> StartConditions;

	UPROPERTY()
	TArray<TObjectPtr<UQuestCondition>> EndConditions;
};