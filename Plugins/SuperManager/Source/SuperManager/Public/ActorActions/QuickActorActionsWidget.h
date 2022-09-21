// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

UENUM(BlueprintType)
enum class E_DuplicationAxis : uint8
{
	EDA_XAxis UMETA (DisplayName = "X Axis"),
	EDA_YAxis UMETA (DisplayName = "Y Axis"),
	EDA_ZAxis UMETA (DisplayName = "Z Axis"),
	EDA_MAX UMETA (DisplayName = "Default Max")
};
/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SelectAllActorsWithSimilarName();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::IgnoreCase;
	
#pragma region ActorBatchDuplication

	UFUNCTION(BlueprintCallable)
	void DuplicateActors();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchDuplication")
	E_DuplicationAxis AxisForDuplication = E_DuplicationAxis::EDA_XAxis;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchDuplication")
	int32 NumberOfDuplicates = 5;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchDuplication")
	float OffsetDist = 300.f;

#pragma endregion

private:
	UPROPERTY()
	class UEditorActorSubsystem* EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
