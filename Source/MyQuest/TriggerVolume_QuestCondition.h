#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "TriggerVolume_QuestCondition.generated.h"

UENUM(BlueprintType)
enum class ETriggerVolumeEvent : uint8
{
	OnEnter UMETA(DisplayName = "On Enter"),
	OnExit  UMETA(DisplayName = "On Exit")
   };

UCLASS()
class MYQUEST_API UTriggerVolume_QuestCondition : public UQuestCondition
{
	GENERATED_BODY()

public:
	virtual void StartCondition() override;
	virtual void StopCondition() override;

protected:
	UFUNCTION()
	void HandleOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category = "Condition")
	FName OtherTag;

	UPROPERTY(EditAnywhere, Category = "Condition")
	ETriggerVolumeEvent TriggerEvent = ETriggerVolumeEvent::OnEnter;
};